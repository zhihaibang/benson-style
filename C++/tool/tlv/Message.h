/*
 * =====================================================================================
 *
 *       Filename:  Message.h
 *
 *    Description:  Work for encoding and encoding message
 *
 *        Version:  1.11
 *        Created:  2014/05/09 17时43分32秒
 *
 *         Author:  baronyuan
 *
 * =====================================================================================
 */

#ifndef __MESSAGE_H__
#define __MESSAGE_H__

// version 1.11
#define kMessageCoderVersion ((0x01 << 8) + 0x11)

#include "endian_util.h"

#include <stdint.h>
#include <string>
#include <vector>
#include <map>

struct MessageAttrValue {
  size_t len;
  const void *value;
};

#pragma pack(1)

struct MessageHeader {
  uint16_t len;
  uint64_t uid;
  uint32_t partition;
  uint32_t seq;
  uint32_t client_ip;
  uint16_t cmd;
  uint16_t sub_cmd;
  uint16_t app_id;
  uint16_t version;
  int8_t status;
};

#pragma pack()

class MessageEncoder {
  public:
    MessageEncoder();
    MessageEncoder(const MessageHeader* h);
    void AddValue(uint16_t key, const void *value, size_t len);
    void AddValue(uint16_t key, const std::string &value);
    void AddValue(uint16_t key, int8_t value);
    void AddValue(uint16_t key, uint8_t value);
    void AddValue(uint16_t key, int16_t value);
    void AddValue(uint16_t key, uint16_t value);
    void AddValue(uint16_t key, int32_t value);
    void AddValue(uint16_t key, uint32_t value);
    void AddValue(uint16_t key, int64_t value);
    void AddValue(uint16_t key, uint64_t value);

    // header setter
    void set_header(const MessageHeader* h);
    void set_uid(uint64_t uid);
    void set_partition(uint32_t partition);
    void set_seq(uint32_t seq);
    void set_client_ip(uint32_t client_ip);
    void set_cmd(uint16_t cmd);
    void set_sub_cmd(uint16_t sub_cmd);
    void set_app_id(uint16_t app_id);
    void set_status(int8_t status);
    // void set_version(uint16_t version);

    void Clear();
    const void *buffer() const;
    size_t len() const;
    MessageHeader *header_();
    
  private:
    void Reset_();
    std::vector<char> buffer_;
};

class MessageDecoder {
  public:
    /*
     * handy func for spp's interface handle_input
     *
     * when buffer not enough, return 0
     * when buffer enough, return length of buffer
     * when buffer invalid (the rest), return -1
     */
    static int Check(const char *buffer, size_t len) {
      if (len < sizeof(MessageHeader)) {
        return 0;
      }

      MessageHeader *h = (MessageHeader *)buffer;

      if (N2H(h->version) != kMessageCoderVersion) {
        return -1;
      }

      if (len < N2H(h->len)) {
        return 0;
      }

      return N2H(h->len);
    }

    MessageDecoder(const void *buffer);
    MessageDecoder(const void *buffer, size_t buffer_len);
    size_t len() const;
    const MessageHeader *header() const;
    const void *buffer() const;

    uint64_t uid() const;
    uint32_t partition() const;
    uint32_t seq() const;
    uint32_t client_ip() const;
    uint16_t cmd() const;
    uint16_t sub_cmd() const;
    uint16_t app_id() const;
    uint16_t version() const;
    int8_t status() const;

    /*
     * do not modify buffer (*p_value) points to !!!
     */
    int GetValue(uint16_t key, const void **p_value, size_t *len);
    int GetValue(uint16_t key, std::string &value);
    int GetValue(uint16_t key, int8_t *value);
    int GetValue(uint16_t key, uint8_t *value);
    int GetValue(uint16_t key, int16_t *value);
    int GetValue(uint16_t key, uint16_t *value);
    int GetValue(uint16_t key, int32_t *value);
    int GetValue(uint16_t key, uint32_t *value);
    int GetValue(uint16_t key, int64_t *value);
    int GetValue(uint16_t key, uint64_t *value);
    int GetValue(uint16_t key, std::vector<MessageAttrValue> &values);
    int GetValue(uint16_t key, std::vector<std::string> &values);
    int GetValue(uint16_t key, std::vector<int8_t> &values);
    int GetValue(uint16_t key, std::vector<uint8_t> &values);
    int GetValue(uint16_t key, std::vector<int16_t> &values);
    int GetValue(uint16_t key, std::vector<uint16_t> &values);
    int GetValue(uint16_t key, std::vector<int32_t> &values);
    int GetValue(uint16_t key, std::vector<uint32_t> &values);
    int GetValue(uint16_t key, std::vector<int64_t> &values);
    int GetValue(uint16_t key, std::vector<uint64_t> &values);
  private:
    const void *body_() const;
    const void *buffer_;
    const MessageHeader *header_;
    typedef std::multimap<uint16_t, MessageAttrValue> AttrMap;
    AttrMap attr_;
};

#endif //!__MESSAGE_H__
