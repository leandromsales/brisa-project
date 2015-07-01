package jbrisa.upnp.controlpoint.model;

import java.util.Date;

public class BrisaCPSubscriptionAnswer {
	
	private String httpStatusMessage;
	private int httpStatusCode;
	private String date;
	private String server;
	private String sid;
	private String timeout;
	
	public String getHttpStatusMessage() {
		return httpStatusMessage;
	}
	
	public int getHttpStatusCode() {
		return httpStatusCode;
	}
	public String getDate() {
		return date;
	}
	public String getServer() {
		return server;
	}
	public String getSid() {
		return sid;
	}
	public String getTimeout() {
		return timeout;
	}

	public void setHttpStatusMessage(String httpStatusMessage) {
		this.httpStatusMessage = httpStatusMessage;
	}

	public void setHttpStatusCode(int httpStatusCode) {
		this.httpStatusCode = httpStatusCode;
	}

	public void setDate(String date) {
		this.date = date;
	}

	public void setServer(String server) {
		this.server = server;
	}

	public void setSid(String sid) {
		this.sid = sid;
	}

	public void setTimeout(String timeout) {
		this.timeout = timeout;
	}

}
