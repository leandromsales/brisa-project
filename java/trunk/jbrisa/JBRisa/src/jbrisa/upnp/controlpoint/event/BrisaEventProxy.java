package jbrisa.upnp.controlpoint.event;

import java.net.URL;
import java.util.ArrayList;

import jbrisa.upnp.controlpoint.model.BrisaCPSubscriptionAnswer;
import jbrisa.utils.BrisaLogger;
import jbrisa.utils.NetworkManager;
import jbrisa.utils._Constants;


public class BrisaEventProxy extends BrisaAbstractEventSubscription {

	private String host; // remote host
	private int port; // remote port

	private String eventSub;
	private String serverName;

	public BrisaEventProxy(ArrayList<String> callbackURLs, String serverName, String host, int port, String eventSub) {

		super(new String(), callbackURLs, -1);

		this.serverName = serverName;
		this.host = host;
		this.port = port;
		this.eventSub = eventSub;
	}

	/**
	 * 
	 * @param timeout
	 */
	//TODO Verificar rejeicao do subscription. Acho que eh pq jah estah inscrito...
	//Verificar também conflito entre resposta da inscricao e repostas de outros aplicativos 
	public void subscribe(final int timeout) {
		
		new Thread(new Runnable() {
			
			@Override
			public void run() {
				
				String subscribeReq = getSubscriptionRequest(timeout);

				String answer = NetworkManager.sendRequest(subscribeReq, host, port);
				
				BrisaCPSubscriptionAnswer subAnswer = new BrisaCPSubscriptionAnswer();
				
				if (NetworkManager.httpOK(answer)) {
					
					subAnswer.setHttpStatusCode(200);
					subAnswer.setHttpStatusMessage("OK");
					
					for (String line : answer.split("\r\n")) {

						if (line.toLowerCase().startsWith("sid")) {
							try {
								sid = line.substring(3);
								subAnswer.setSid(sid);
							} catch (Exception e) {
							}
						}
						
						else if (line.toLowerCase().startsWith("date")) {
							try {
								subAnswer.setDate(line.substring(6));
							} catch (Exception e) {
							}
						}
						
						else if (line.toLowerCase().startsWith("server")) {
							try {
								subAnswer.setServer(line.substring(8));
							} catch (Exception e) {
							}
						}
						
						else if (line.toLowerCase().startsWith("timeout")) {
							try {
								subAnswer.setTimeout(line.substring(9));
							} catch (Exception e) {
							}
						}
					}
				} else if (NetworkManager.httpError500(answer)) {
						
					subAnswer.setHttpStatusCode(503);
					subAnswer.setHttpStatusMessage("Not Implemented");
				}
				
				setChanged();
				notifyObservers(subAnswer);
			}
		}).start();
	}

	/**
	 * 
	 */
	public void unsubscribe() {
		String unSubscribeReq = getUnsubscriptionRequest();

		BrisaLogger.logger.info("Unsubscription: \n"
				+ unSubscribeReq);
		
		String answer = NetworkManager.sendRequest(unSubscribeReq, host, port);
		
		if(NetworkManager.httpOK(answer)) {
			BrisaLogger.logger.info("Unsubscription on "+ host + ":" + port + " OK!");
		} else {
			BrisaLogger.logger.info("Unsubscription on "+ host + ":" + port + " failed!");
		}
	}

	/**
	 * 
	 */
	public void renew(int newTimeout) {
		String renewReq = getRenewRequest(newTimeout);
		BrisaLogger.logger.info("Renew: \n" + renewReq);
		
		String answer = NetworkManager.sendRequest(renewReq, host, port);
		
		if(NetworkManager.httpOK(answer)) {
			BrisaLogger.logger.info("Renew Subscription on "+ host + ":" + port + " OK!");
		} else {
			BrisaLogger.logger.info("Renew Subscription on "+ host + ":" + port + " failed!");
		}
	}

	/**
	 * 
	 * @param timeout
	 * @return
	 */
	private String getSubscriptionRequest(int timeout) {

		// Our URL for receiving notifications
		URL url = this.getUrl();

		String request = _Constants.UPNP_SUBSCRIPTION_MESSAGE
				.replace("%1", getFullPath(host, port, eventSub))
				.replace("%2", host) 
				.replace("%3", Integer.toString(port))
				.replace("%4", getUserAgent(serverName))
				.replace("%5", getCallBack(url))
				.replace("%6", (timeout > 0) ? "Second-" + timeout : "INFINITE");

		return request;
	}

	/**
	 * 
	 * @return
	 */
	private String getUnsubscriptionRequest() {

		String request = _Constants.UPNP_UNSUBSCRIPTION_MESSAGE
				.replace("%1", getFullPath(host, port, eventSub))
				.replace("%2", host).replace("%3", Integer.toString(port))
				.replace("%4", getSid());

		return request;
	}

	/**
	 * 
	 * @param timeout
	 * @return
	 */
	private String getRenewRequest(int timeout) {

		if (this.getSid().isEmpty()) {
			BrisaLogger.logger.warning("Renew failed: SID field not filled.");
			return null;
		}

		String request = _Constants.UPNP_RENEW_SUBSCRIPTION_MESSAGE
				.replace("%1", getFullPath(host, port, eventSub))
				.replace("%2", host)
				.replace("%3", Integer.toString(port))
				.replace("%4", getSid())
				.replace("%5", (timeout > 0) ? "Second-" + timeout : "INFINITE");

		return request;
	}

	/**
	 * Get full path like http://ip:port/service
	 * @param ip
	 * @param port
	 * @param rpath
	 * @return
	 */
	private String getFullPath(String ip, int port, String rpath) {
		String fullpath = "http://" + ip + ":" + port;
		if (rpath.startsWith("/")) {
			fullpath += rpath;
		} else {
			fullpath += "/" + rpath;
		}
		return fullpath;
	}
	/*
	private String getRelativePath(String rpath) {

		String fullpath = "";
		
		if (rpath.startsWith("/")) {
			fullpath += rpath;
		} else {
			fullpath += "/" + rpath;
		}
		return fullpath;
	}
	*/

	/**
	 * 
	 * @param product
	 * @return
	 */
	private String getUserAgent(String product) {
		return System.getProperty("os.name") + "/"
				+ System.getProperty("os.version") + " UPnP/1.1 " + product;
	}

	/**
	 * 
	 * @param url
	 * @return
	 */
	private String getCallBack(URL url) {
		return "<http://" + url.getHost() + ":" + url.getPort() + ">";
	}
	

}
