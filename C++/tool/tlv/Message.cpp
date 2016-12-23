/*
 * =====================================================================================
 *
 *       Filename:  Message.cpp
 *
 *    Description:  Work for encoding and encoding message
 *
 *
 * =====================================================================================
 */

#include "Message.h"
#include "endian_util.h"

#include <arpa/inet.h>
#include <algorithm>
#include <iostream>
#include <iterator>

using namespace std;

#pragma pack(1)

struct MessageAttrHeader {
  uint16_t key;
  uint16_t len;
};

#pragma pack()

MessageEncoder::MessageEncoder()
  :buffer_(sizeof(MessageHeader)) {
  Reset_();
}

MessageEncoder::MessageEncoder(const MessageHeader* h) {
  copy((char *)h, (char *)h + sizeof(MessageHeader), back_inserter(buffer_));
  Reset_();
}

MessageHeader *MessageEncoder::header_() {
  return (MessageHeader *)buffer_.data();
}

void MessageEncoder::Reset_() {
  header_()->len = H2N((uint16_t)sizeof(MessageHeader));
  header_()->version = H2N((uint16_t)kMessageCoderVersion);
}

void MessageEncoder::AddValue(uint16_t key, const void *value, size_t len) {
  MessageAttrHeader h = {H2N(key), H2N((uint16_t)len)};
  copy((char *)&h, (char *)&h + sizeof(MessageAttrHeader), back_inserter(buffer_));
  copy((char *)value, (char *)value + len, back_inserter(buffer_));
  header_()->len = H2N((uint16_t)buffer_.size());
}

void MessageEncoder::AddValue(uint16_t key, const string &value) {
  AddValue(key, value.data(), value.size());
}

void MessageEncoder::AddValue(uint16_t key, int8_t value) {
  AddValue(key, &value, sizeof(int8_t));
}

void MessageEncoder::AddValue(uint16_t key, uint8_t value) {
  AddValue(key, &value, sizeof(uint8_t));
}

void MessageEncoder::AddValue(uint16_t key, int16_t value) {
  value = H2N(value);
  AddValue(key, &value, sizeof(int16_t));
}

void MessageEncoder::AddValue(uint16_t key, uint16_t value) {
  value = H2N(value);
  AddValue(key, &value, sizeof(uint16_t));
}

void MessageEncoder::AddValue(uint16_t key, int32_t value) {
  value = H2N(value);
  AddValue(key, &value, sizeof(int32_t));
}

void MessageEncoder::AddValue(uint16_t key, uint32_t value) {
  value = H2N(value);
  AddValue(key, &value, sizeof(uint32_t));
}

void MessageEncoder::AddValue(uint16_t key, int64_t value) {
  value = H2N(value);
  AddValue(key, &value, sizeof(int64_t));
}

void MessageEncoder::AddValue(uint16_t key, uint64_t value) {
  value = H2N(value);
  AddValue(key, &value, sizeof(uint64_t));
}

void MessageEncoder::set_header(const MessageHeader *h) {
  copy((char *)h, (char *)h + sizeof(MessageHeader), buffer_.begin());
  header_()->len = H2N((uint16_t)buffer_.size());
  header_()->version = H2N((uint16_t)kMessageCoderVersion);
}

void MessageEncoder::set_uid(uint64_t uid) {
  header_()->uid = H2N(uid);
}

void MessageEncoder::set_partition(uint32_t partition) {
  header_()->partition = H2N(partition);
}

void MessageEncoder::set_seq(uint32_t seq) {
  header_()->seq = H2N(seq);
}

void MessageEncoder::set_client_ip(uint32_t client_ip) {
  header_()->client_ip = H2N(client_ip);
}

void MessageEncoder::set_cmd(uint16_t cmd) {
  header_()->cmd = H2N(cmd);
}

void MessageEncoder::set_sub_cmd(uint16_t sub_cmd) {
  header_()->sub_cmd = H2N(sub_cmd);
}

void MessageEncoder::set_app_id(uint16_t app_id) {
  header_()->app_id = H2N(app_id);
}

// void MessageEncoder::set_version(uint16_t version) {
  // header_()->version = version;
// }

void MessageEncoder::set_status(int8_t status) {
  header_()->status = status;
}

void MessageEncoder::Clear() {
  buffer_.resize(sizeof(MessageHeader));
  Reset_();
}

const void *MessageEncoder::buffer() const {
  return buffer_.data();
}

size_t MessageEncoder::len() const {
  return buffer_.size();
}

MessageDecoder::MessageDecoder(const void *buffer)
  :buffer_(buffer),
   header_((const MessageHeader *)buffer
) {
  const char *body = (const char *)body_();
  size_t body_len = N2H(header_->len) - sizeof(MessageHeader);
  size_t offset = 0;

  while (offset + sizeof(MessageAttrHeader) <= body_len) {
    const MessageAttrHeader *h = (const MessageAttrHeader *)(body + offset);
    uint16_t val_len = N2H(h->len);

    if (offset + sizeof(MessageAttrHeader) + val_len > body_len) {
      break;
    }

    MessageAttrValue value = {val_len, (const char *)h + sizeof(MessageAttrHeader)};
    attr_.insert(pair<uint16_t, MessageAttrValue>(N2H(h->key), value));
    offset += sizeof(MessageAttrHeader) + val_len;
  }
}

MessageDecoder::MessageDecoder(const void *buffer, size_t buffer_len)
  :buffer_(buffer),
   header_((const MessageHeader *)buffer
) {
  const char *body = (const char *)body_();
  if (N2H(header_->len) > buffer_len)
  {
    return;
  }

  size_t body_len = N2H(header_->len) - sizeof(MessageHeader);
  size_t offset = 0;

  while (offset + sizeof(MessageAttrHeader) <= body_len) {
    const MessageAttrHeader *h = (const MessageAttrHeader *)(body + offset);
    uint16_t val_len = N2H(h->len);

    if (offset + sizeof(MessageAttrHeader) + val_len > body_len) {
      break;
    }

    MessageAttrValue value = {val_len, (const char *)h + sizeof(MessageAttrHeader)};
    attr_.insert(pair<uint16_t, MessageAttrValue>(N2H(h->key), value));
    offset += sizeof(MessageAttrHeader) + val_len;
  }
}

size_t MessageDecoder::len() const {
  return N2H(header_->len);
}

const MessageHeader *MessageDecoder::header() const {
  return header_;
}

const void *MessageDecoder::buffer() const {
  return buffer_;
}

uint64_t MessageDecoder::uid() const {
  return N2H(header_->uid);
}

uint32_t MessageDecoder::partition() const {
  return N2H(header_->partition);
}

uint32_t MessageDecoder::seq() const {
  return N2H(header_->seq);
}

uint32_t MessageDecoder::client_ip() const {
  return N2H(header_->client_ip);
}

uint16_t MessageDecoder::cmd() const {
  return N2H(header_->cmd);
}

uint16_t MessageDecoder::sub_cmd() const {
  return N2H(header_->sub_cmd);
}

uint16_t MessageDecoder::app_id() const {
  return N2H(header_->app_id);

}

uint16_t MessageDecoder::version() const {
  return N2H(header_->version);
}

int8_t MessageDecoder::status() const {
  return header_->status;
}

int MessageDecoder::GetValue(uint16_t key, const void **p_value, size_t *len) {
  AttrMap::const_iterator it = attr_.find(key);

  if (it == attr_.end()) {
    return -1;
  }

  *p_value = it->second.value;
  *len = it->second.len;
  return 0;
}

int MessageDecoder::GetValue(uint16_t key, string &value) {
  const void *buf = NULL;
  size_t len = 0;

  if (-1 == GetValue(key, &buf, &len)) {
    return -1;
  }

  value.assign((const char *)buf, len);
  return 0;
}

int MessageDecoder::GetValue(uint16_t key, int8_t *value) {
  const void *buf = NULL;
  size_t len = 0;

  if (GetValue(key, &buf, &len) || len != sizeof(int8_t)) {
    return -1;
  }

  *value = *(int8_t *)buf;
  return 0;
}

int MessageDecoder::GetValue(uint16_t key, uint8_t *value) {
  const void *buf = NULL;
  size_t len = 0;

  if (GetValue(key, &buf, &len) || len != sizeof(uint8_t)) {
    return -1;
  }

  *value = *(uint8_t *)buf;
  return 0;
}

int MessageDecoder::GetValue(uint16_t key, int16_t *value) {
  const void *buf = NULL;
  size_t len = 0;

  if (GetValue(key, &buf, &len) || len != sizeof(int16_t)) {
    return -1;
  }

  *value = N2H(*(int16_t *)buf);
  return 0;
}

int MessageDecoder::GetValue(uint16_t key, uint16_t *value) {
  const void *buf = NULL;
  size_t len = 0;

  if (GetValue(key, &buf, &len) || len != sizeof(uint16_t)) {
    return -1;
  }

  *value = N2H(*(int16_t *)buf);
  return 0;
}

int MessageDecoder::GetValue(uint16_t key, int32_t *value) {
  const void *buf = NULL;
  size_t len = 0;

  if (GetValue(key, &buf, &len) || len != sizeof(int32_t)) {
    return -1;
  }

  *value = N2H(*(int32_t *)buf);
  return 0;
}

int MessageDecoder::GetValue(uint16_t key, uint32_t *value) {
  const void *buf = NULL;
  size_t len = 0;

  if (GetValue(key, &buf, &len) || len != sizeof(uint32_t)) {
    return -1;
  }

  *value = N2H(*(uint32_t *)buf);
  return 0;
}

int MessageDecoder::GetValue(uint16_t key, int64_t *value) {
  const void *buf = NULL;
  size_t len = 0;

  if (GetValue(key, &buf, &len) || len != sizeof(int64_t)) {
    return -1;
  }

  *value = N2H(*(int64_t *)buf);
  return 0;
}

int MessageDecoder::GetValue(uint16_t key, uint64_t *value) {
  const void *buf = NULL;
  size_t len = 0;

  if (GetValue(key, &buf, &len) || len != sizeof(uint64_t)) {
    return -1;
  }

  *value = N2H(*(uint64_t *)buf);
  return 0;
}

int MessageDecoder::GetValue(uint16_t key, std::vector<MessageAttrValue> &values) {
  std::pair<AttrMap::const_iterator, AttrMap::const_iterator> it_pair = attr_.equal_range(key);
  AttrMap::const_iterator it = it_pair.first;

  if (it_pair.first == it_pair.second) {
    return -1;
  }

  while (it != it_pair.second) {
    values.push_back(it->second);
    ++it;
  }

  return 0;
}

int MessageDecoder::GetValue(uint16_t key, std::vector<string> &values) {
  std::pair<AttrMap::const_iterator, AttrMap::const_iterator> it_pair = attr_.equal_range(key);
  AttrMap::const_iterator it = it_pair.first;

  if (it_pair.first == it_pair.second) {
    return -1;
  }

  while (it != it_pair.second) {
    values.push_back(string((char *)it->second.value, it->second.len));
    ++it;
  }

  return 0;
}

int MessageDecoder::GetValue(uint16_t key, std::vector<int8_t> &values) {
  std::pair<AttrMap::const_iterator, AttrMap::const_iterator> it_pair = attr_.equal_range(key);
  AttrMap::const_iterator it = it_pair.first;

  if (it_pair.first == it_pair.second) {
    return -1;
  }

  while (it != it_pair.second) {
    if (sizeof(int8_t) == it->second.len) {
      values.push_back(*(int8_t *)it->second.value);
    }

    ++it;
  }

  return 0;
}

int MessageDecoder::GetValue(uint16_t key, std::vector<uint8_t> &values) {
  std::pair<AttrMap::const_iterator, AttrMap::const_iterator> it_pair = attr_.equal_range(key);
  AttrMap::const_iterator it = it_pair.first;

  if (it_pair.first == it_pair.second) {
    return -1;
  }

  while (it != it_pair.second) {
    if (sizeof(uint8_t) == it->second.len) {
      values.push_back(*(uint8_t *)it->second.value);
    }

    ++it;
  }

  return 0;
}

int MessageDecoder::GetValue(uint16_t key, std::vector<int16_t> &values) {
  std::pair<AttrMap::const_iterator, AttrMap::const_iterator> it_pair = attr_.equal_range(key);
  AttrMap::const_iterator it = it_pair.first;

  if (it_pair.first == it_pair.second) {
    return -1;
  }

  while (it != it_pair.second) {
    if (sizeof(int16_t) == it->second.len) {
      values.push_back(N2H(*(int16_t *)it->second.value));
    }

    ++it;
  }

  return 0;
}

int MessageDecoder::GetValue(uint16_t key, std::vector<uint16_t> &values) {
  std::pair<AttrMap::const_iterator, AttrMap::const_iterator> it_pair = attr_.equal_range(key);
  AttrMap::const_iterator it = it_pair.first;

  if (it_pair.first == it_pair.second) {
    return -1;
  }

  while (it != it_pair.second) {
    if (sizeof(uint16_t) == it->second.len) {
      values.push_back(N2H(*(uint16_t *)it->second.value));
    }

    ++it;
  }

  return 0;
}

int MessageDecoder::GetValue(uint16_t key, std::vector<int32_t> &values) {
  std::pair<AttrMap::const_iterator, AttrMap::const_iterator> it_pair = attr_.equal_range(key);
  AttrMap::const_iterator it = it_pair.first;

  if (it_pair.first == it_pair.second) {
    return -1;
  }

  while (it != it_pair.second) {
    if (sizeof(int32_t) == it->second.len) {
      values.push_back(N2H(*(int32_t *)it->second.value));
    }

    ++it;
  }

  return 0;
}

int MessageDecoder::GetValue(uint16_t key, std::vector<uint32_t> &values) {
  std::pair<AttrMap::const_iterator, AttrMap::const_iterator> it_pair = attr_.equal_range(key);
  AttrMap::const_iterator it = it_pair.first;

  if (it_pair.first == it_pair.second) {
    return -1;
  }

  while (it != it_pair.second) {
    if (sizeof(uint32_t) == it->second.len) {
      values.push_back(N2H(*(uint32_t *)it->second.value));
    }

    ++it;
  }

  return 0;
}

int MessageDecoder::GetValue(uint16_t key, std::vector<int64_t> &values) {
  std::pair<AttrMap::const_iterator, AttrMap::const_iterator> it_pair = attr_.equal_range(key);
  AttrMap::const_iterator it = it_pair.first;

  if (it_pair.first == it_pair.second) {
    return -1;
  }

  while (it != it_pair.second) {
    if (sizeof(int64_t) == it->second.len) {
      values.push_back(N2H(*(int64_t *)it->second.value));
    }

    ++it;
  }

  return 0;
}

int MessageDecoder::GetValue(uint16_t key, std::vector<uint64_t> &values) {
  std::pair<AttrMap::const_iterator, AttrMap::const_iterator> it_pair = attr_.equal_range(key);
  AttrMap::const_iterator it = it_pair.first;

  if (it_pair.first == it_pair.second) {
    return -1;
  }

  while (it != it_pair.second) {
    if (sizeof(uint64_t) == it->second.len) {
      values.push_back(N2H(*(uint64_t *)it->second.value));
    }

    ++it;
  }

  return 0;
}

const void *MessageDecoder::body_() const {
  return (const char *)buffer_ + sizeof(MessageHeader);
}
