<?php

/**
* DES加密，使用CBC方式，偏移量为0
*/
class DES {

    var $key;
    var $iv; //偏移量
  
    public function __construct($key, $iv=0)
    {
        $this->key = $key;
        if($iv == 0)
        {
            $this->iv = $key;
        }
        else
        {
            $this->iv = $iv;
        }
    }
  
    //加密
    public function encrypt($str)
    {       
        $size = mcrypt_get_block_size ( MCRYPT_DES, MCRYPT_MODE_CBC);
        $str = $this->pkcs5Pad ($str, $size);
        $data=mcrypt_cbc(MCRYPT_DES, $this->key, $str, MCRYPT_ENCRYPT, $this->iv);
        return base64_encode($data);
    }
  
    //解密
    public function decrypt($str)
    {
        $str = base64_decode ($str);
        $str = mcrypt_cbc(MCRYPT_DES, $this->key, $str, MCRYPT_DECRYPT, $this->iv);
        $str = $this->pkcs5Unpad($str);
        return $str;
    }
  
    private function pkcs5Pad($text, $blocksize)
    {
        $pad = $blocksize - (strlen ($text) % $blocksize);
        return $text . str_repeat (chr ($pad), $pad);
    }
  
    private function pkcs5Unpad($text)
    {
        $pad = ord ( $text {strlen ($text) - 1} );
        if ($pad > strlen ($text))
            return false;
        if (strspn ( $text, chr ($pad), strlen ( $text ) - $pad ) != $pad)
            return false;
        return substr ( $text, 0, - 1 * $pad );
    }
}