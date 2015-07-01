package jbrisa.utils;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.BindException;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.util.Enumeration;

public class NetworkManager {
	
	/**
	 * 
	 * @return a IP Address
	 * @throws SocketException
	 */
	public static String getIPAddress() throws SocketException {
		
			String ipAddress = new String();
			
			boolean bindAddress = false;
			Enumeration<NetworkInterface> nis = NetworkInterface
					.getNetworkInterfaces();

			while (nis.hasMoreElements()) {
				NetworkInterface ni = (NetworkInterface) nis.nextElement();
				Enumeration<InetAddress> ias = ni.getInetAddresses();
				while (ias.hasMoreElements()) {
					InetAddress ia = (InetAddress) ias.nextElement();
					String h = ia.getHostAddress();
					if (!h.contains(":") && !h.contains("127.0.1.1")
							&& !h.contains("127.0.0.1")) {
						ipAddress = ia.getHostAddress();
						bindAddress = true;
						break;
					}
				}
				if (!bindAddress) {
					ipAddress = "127.0.0.1"; // se estiver sem rede, use
													// localhost
				}
			}
			
			return ipAddress;
}
	
	
	/**
	 * @return a random open port
	 */
	public static Integer getPort() {
		//Generate a port number in range [49152,65535]
		Integer port = randomNumber(49152,65535);
		
		while(!portIsOpen(port)) {
			port = randomNumber(49152,65535);
		}
		
		return port;
	}
	
	/**
	 * Verifica se a porta está em uso
	 * @param port
	 * @return
	 */
	private static boolean portIsOpen(Integer port) {
		try {
			ServerSocket s = new ServerSocket(port);
			s.close();
			return true;
		} catch (BindException e) {
			return false;
		} catch (IOException e) {
			return false;
		}
		
	}
	
	/**
	 * Gets a random number in range [x,y]
	 * @param x
	 * @param y
	 * @return A random number in range [x,y]
	 */
	private static Integer randomNumber(Integer x, Integer y) {
		return (int)(x + (x-y+1)*Math.random());
	}
	
	 /**
	 * Sends request to host:port
	 * 
	 * @param request
	 * @param host
	 * @param port
	 */
	public static String sendRequest(String request, String host, int port) {

		Socket clientSocket;
		String answer = "";
		try {
			clientSocket = new Socket(host, port);
			DataOutputStream out = new DataOutputStream(
					clientSocket.getOutputStream());
			BufferedReader in = new BufferedReader(new InputStreamReader(
					clientSocket.getInputStream()));

			out.writeBytes(request);
			
			String line = in.readLine();
			
			while(line != null && !line.equalsIgnoreCase("\r\n")) {
				answer += line + "\r\n";
				line = in.readLine();
			}

			//System.out.println("RESPOSTA: \n" + answer + "-----\n");
			
		} catch (UnknownHostException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}

		return answer;
	}
	
	/**
	 * Verify if is HTTP 200 OK!
	 * @param a
	 * @return
	 */
	public static boolean httpOK(String a) {
		//if(a.startsWith());
		String http10 = "HTTP/1.0 200 OK";
		String http11 = "HTTP/1.1 200 OK";
		
		if(a.toUpperCase().startsWith(http10) || a.toUpperCase().startsWith(http11)) {
			return true;
		}
		return false;
	}
	
	public static boolean httpError500(String a) {
		//if(a.startsWith());
		String http10 = "HTTP/1.0 5";
		String http11 = "HTTP/1.1 5";
		
		if(a.toUpperCase().startsWith(http10) || a.toUpperCase().startsWith(http11)) {
			return true;
		}
		return false;
	}
}
