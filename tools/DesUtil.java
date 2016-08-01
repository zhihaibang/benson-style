package zhb.des;

import java.security.Key;
import java.security.spec.AlgorithmParameterSpec;
import javax.crypto.Cipher;
import javax.crypto.SecretKeyFactory;
import javax.crypto.spec.DESKeySpec;
import javax.crypto.spec.IvParameterSpec;
import it.sauronsoftware.base64.Base64;
 
/**
* DES加密，使用CBC方式，偏移量为0	
*/
public class DesUtil {

	 public static final String ALGORITHM_DES = "DES/CBC/PKCS5Padding";
	 private static byte[] DESIV = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

	 /**
	  * DES算法，加密
	  *
	  * @param data 待加密字符串
	  * @param key 加密私钥，长度不能够小于8位
	  * @return 加密后的字符串，一般结合Base64编码使用
	  * @throws CryptException 异常
	  */
	 public static String encrypt(String key,String data) throws Exception
	 {
        DESKeySpec dks = new DESKeySpec(key.getBytes());
        SecretKeyFactory keyFactory = SecretKeyFactory.getInstance("DES");
	   //key的长度不能够小于8位字节
	   Key secretKey = keyFactory.generateSecret(dks);
	   Cipher cipher = Cipher.getInstance(ALGORITHM_DES);
	   IvParameterSpec iv = new IvParameterSpec(DESIV);
	   AlgorithmParameterSpec paramSpec = iv;
	   cipher.init(Cipher.ENCRYPT_MODE, secretKey, paramSpec);
	   byte[] bytes = cipher.doFinal(data.getBytes());
	   return new String(Base64.encode(bytes));
	 }
	  
	 /**
	  * DES算法，解密
	  *
	  * @param data 待解密字符串
	  * @param key 解密私钥，长度不能够小于8位
	  * @return 解密后的字符串
	  * @throws Exception 异常
	  */
	 public static String decrypt(String key,String data) throws Exception
	 {
	        DESKeySpec dks = new DESKeySpec(key.getBytes());
	        SecretKeyFactory keyFactory = SecretKeyFactory.getInstance("DES");
		   //key的长度不能够小于8位字节
		   Key secretKey = keyFactory.generateSecret(dks);
		   Cipher cipher = Cipher.getInstance(ALGORITHM_DES);
		   IvParameterSpec iv = new IvParameterSpec(DESIV);
		   AlgorithmParameterSpec paramSpec = iv;
		   cipher.init(Cipher.DECRYPT_MODE, secretKey,paramSpec);	   
		   byte[] bytes =  cipher.doFinal(Base64.decode(data.getBytes()));
		   return new String(bytes);
	 }
	  
	 public static void main(String[] args) throws Exception
	 {
		String str = "hello world,zhb!";
		String key = "very important data";
		String m_data = encrypt(key,str);
		
		System.out.println(str);
	    System.out.println(m_data);
	    
		String data = decrypt(key,m_data);
	    
	    System.out.println(data);
	 }
}
