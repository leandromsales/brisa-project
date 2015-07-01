package jbrisa.upnp.controlpoint.core;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketTimeoutException;
import java.util.Observable;

import jbrisa.utils.BrisaLogger;
import jbrisa.utils._Constants;


/**
 * Create a new BrisaMSearchClientCP with the desired service type and mx
 * values, and call start() to begin sending discovery messages to possible
 * devices in the multicast group.
 * 
 * @author Wendell Silva
 * 
 */
public class BrisaMSearchClient extends Observable{

	public static final String DEFAULT_SEARCH_TYPE = "ssdp:all";
	
	private static int DEFAULT_SEARCH_TIME = 5;
	
	private String type;

	private int mx;

	private DatagramSocket datagramSocket;

	protected BrisaMSearchClient() {
		this(DEFAULT_SEARCH_TYPE);
	}

	protected BrisaMSearchClient(String searchType) {
		this(searchType, DEFAULT_SEARCH_TIME);
	}

	protected BrisaMSearchClient(String searchType, int mx) {
		type = searchType;
		this.mx = mx;
	}

	// =========================================================================
	// PUBLIC METHODS===========================================================
	// =========================================================================

	/**
	 * 
	 */
	protected void start() {

	}
	
	protected void setSearchType(String type){
		this.type = type;
	}
	
	protected void setMx(int mx){
		this.mx = mx;
	}

	/**
	 * 
	 */
	protected synchronized void discover() {

		try {

			byte[] buffer = createSearchMessage().getBytes();
			DatagramPacket question = new DatagramPacket(buffer, buffer.length,
					InetAddress.getByName(_Constants.DEFAULT_MSEARCH_IP), _Constants.DEFAULT_MSEARCH_PORT);

			// Setting the timeout value for the send udp datagram;
			int timeOut = (mx > 0 ? mx : DEFAULT_SEARCH_TIME) * 1000;
			datagramSocket = new DatagramSocket();
			datagramSocket.setSoTimeout(timeOut);
			BrisaLogger.logger.info("BrisaMSearch search timeout set to "+ timeOut);

			// Send datagram;
			datagramSocket.send(question);
			
			BrisaLogger.logger.info("BrisaMSearch discover message sent");

			final DatagramPacket answer = new DatagramPacket(
					new byte[_Constants.DEFAULT_DATAGRAM_SIZE],
					_Constants.DEFAULT_DATAGRAM_SIZE);

			// While the timeout exception isn't fired;
			new Thread(new Runnable() {
				
				@Override
				public void run() {
					while (true) {
						
						try {
							synchronized (datagramSocket) {
								datagramSocket.receive(answer);
							}
						} catch (IOException e) {
							//e.printStackTrace();
						}

						if (answer.getAddress() != null) {

							String msg = new String(answer.getData(), 0, answer.getLength());
							notifyAllObservers(msg);
						}
				}
			}   
		}).start();

		// When the timeout event occurs or there's no connection;
		} catch (SocketTimeoutException e) {

			datagramSocket.close();

			BrisaLogger.logger.info("BrisaMSearch message timeout happened. "+ e.getMessage());
			
		}catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	private void notifyAllObservers(String string){
		
		this.setChanged();
		this.notifyObservers(string);
	}

	/**
	 * 
	 */
	protected void stop() {
		// TODO Stop
	}

	// =========================================================================
	// PRIVATE METHODS==========================================================
	// =========================================================================
	/*
	private void dispatchConnectionFaultError(SocketException e) {
		
		setChanged();
		notifyObservers(e.getMessage());
	}
	*/
	
	private String createSearchMessage(){
		
		String discoverMessage = new String(_Constants.UPNP_MSEARCH_DISCOVER_BASE);
		discoverMessage += getMXLine();
		discoverMessage += getSTLine();
		discoverMessage += getEndLine();
		
		return discoverMessage;
	}
	
	private String getMXLine() {
		return "MX: " + Integer.toString(mx) + "\r\n";
	}

	private String getSTLine() {
		return "ST: " + type + "\r\n";
	}

	private String getEndLine() {
		return "\r\n";
	}

}
