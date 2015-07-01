package jbrisa.webserver.simplewebserver;

import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.net.Socket;
import java.util.StringTokenizer;

/**
 * Responde as requisicoes HTTP
 * 
 * @author wendell
 *
 */
@Deprecated
public class HTTPManager {

	private Socket clientSocket;
	private DataOutputStream out;

	public HTTPManager(Socket clientSocket)  {
		this.clientSocket = clientSocket;
	}

	/**
	 * 
	 * @param request
	 *            Mensagem do cliente
	 * @param out
	 *            Stream de saida para o cliente
	 * @throws IOException 
	 */

	public void respondRequest(String request) throws IOException {

		this.out = new DataOutputStream(clientSocket.getOutputStream());
		
		if (request.startsWith("GET")) {
			this.respondGET(request);
		} else if (request.startsWith("POST")) {
			this.respondPOST(request);
		}

	}

	/**
	 * Reponde a Requisicoes GET 
	 * 
	 * Checar todos em
	 * http://en.wikipedia.org/wiki/Internet_media_type
	 */
	private void respondGET(String request) {
		String fileName = new String();

		StringTokenizer requestTokenizer = new StringTokenizer(request, "\r\n");
		String line = requestTokenizer.nextToken();
		StringTokenizer linetokenizer = new StringTokenizer(line);
		linetokenizer.nextToken(); //Pula String 'GET'

		try {
			// Tratar para outros tipos...
			
			fileName = linetokenizer.nextToken();

			if (fileName.startsWith("/"))
				fileName = fileName.substring(1);

			File file = new File(fileName);
			int numberBytes = (int) file.length();
			System.out.println(file.length() + ": " + numberBytes);
			FileInputStream fileIn = new FileInputStream(fileName);

			byte[] arquivoEmBytes = new byte[numberBytes];
			fileIn.read(arquivoEmBytes);

			out.writeBytes("HTTP/1.0 200 Document Follows\r\n");

			if (fileName.endsWith(".jpg"))
				out.writeBytes("Content-Type: image/jpeg\r\n");
			else if (fileName.endsWith(".gif"))
				out.writeBytes("Content-Type: image/jpeg\r\n");

			else if (fileName.endsWith(".htm"))
				out.writeBytes("Content-Type: text/htm\r\n");

			else if (fileName.endsWith(".html"))
				out.writeBytes("Content-Type: text/html\r\n");

			else if (fileName.endsWith(".xml"))
				out.writeBytes("Content-Type: text/xml\r\n");

			else
				out.writeBytes("Content-Type: file/other\r\n");

			out.writeBytes("Content-Length: " + numberBytes + "\r\n");
			out.writeBytes("\r\n");

			out.write(arquivoEmBytes, 0, numberBytes);

		} catch (IOException e) {
			//Retornar Erro HTTP
			e.printStackTrace();
		}
	}

	/**
	 * 
	 * @param request
	 */
	private void respondPOST(String request) {
		System.out.println("POST \n:" + request);
	}

}
