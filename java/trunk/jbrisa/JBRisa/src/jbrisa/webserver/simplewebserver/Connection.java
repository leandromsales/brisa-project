package jbrisa.webserver.simplewebserver;

import java.io.IOException;
import java.net.Socket;
import java.util.Scanner;

/**
 * Gerencia a conexao dos cliente com o servidor
 * 
 * @author wendell
 *
 */
@Deprecated
public class Connection implements Runnable {

	private Socket clientSocket;
	private HTTPManager http;

	/**
	 * 
	 * @param client
	 *            Socket do cliente
	 */
	public Connection(Socket client) {
		this.clientSocket = client;
		this.http = new HTTPManager(this.clientSocket);
	}

	/**
	 * Envia a requisicao para o HTTPManager
	 */
	@Override
	public void run() {

		System.out.println("Nova conexao com o cliente "
				+ clientSocket.getInetAddress().getHostAddress() + "\n");

		try {
			Scanner input = new Scanner(clientSocket.getInputStream());

			String request = new String();
			String line = new String();

			while (input.hasNextLine()) {
				line = input.nextLine();

				if (line.length() == 0) {
					request += "\r\n";
					break;
				} else {
					request += line + "\r\n";
				}
				//request += line + "\r\n";
			}

			http.respondRequest(request);

			clientSocket.close();
		} catch (IOException e) {
			e.printStackTrace();
		}

	}


}
