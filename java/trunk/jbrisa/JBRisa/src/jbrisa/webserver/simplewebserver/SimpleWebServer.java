package jbrisa.webserver.simplewebserver;

import java.net.ServerSocket;
import java.net.Socket;
import java.util.Observer;

import jbrisa.upnp.device.core.BrisaDevice;
import jbrisa.utils.BrisaLogger;
import jbrisa.webserver.BrisaWebServer;


/**
 * 
 * @author wendell
 *
 */
@Deprecated
public class SimpleWebServer implements BrisaWebServer {

	private int port;
	private ServerSocket serverSocket;
	private Thread webServerThread;
	private boolean running;

	public SimpleWebServer() {
		this.running = false;
	}
	
	@Override
	public int getPort() {
		return port;
	}


	@Override
	public void setPort(int port) {
		this.port = port;
	}


	/**
	 * Start JBRisa Web Server
	 */
	@Override
	public void start() {

		if (!isRunning()) {

			webServerThread = new Thread(new Runnable() {

				@Override
				public void run() {
					try {
						serverSocket = new ServerSocket(port);
						running = true;
						while (running) {
							Socket clientSocket = serverSocket.accept();
							Connection newConnection = new Connection(
									clientSocket);
							Thread t = new Thread(newConnection);

							t.start();
						}

					} catch (Exception e) {
						running = false;
						e.printStackTrace();
					}
				}
			});

			running = true;
			webServerThread.start();

		} else {

			BrisaLogger.logger.warning("JBrisa Web Server: Already running!");
		}
	}

	/**
	 * Stop JBrisa Web Server
	 */
	@Override
	public void stop() {
		if (isRunning()) {
			try {
				webServerThread.join();
				webServerThread.interrupt();
			} catch (InterruptedException e) {
				e.printStackTrace();
			} catch (Exception e) {
				e.printStackTrace();
			}
		} else {
			BrisaLogger.logger.warning("JBrisa Web Server: Already stopped!");
		}
	}

	/**
	 * 
	 * @return is Running
	 */
	@Override
	public boolean isRunning() {
		return this.running;
	}


	@Override
	public void addNewObserver(Observer o) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void setBrisaDevice(BrisaDevice brisaDevice) {
		// TODO Auto-generated method stub
		
	}

}
