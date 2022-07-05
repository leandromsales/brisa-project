package jbrisa.upnp.controlpoint.event;

import java.net.MalformedURLException;
import java.net.URL;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.GregorianCalendar;
import java.util.Observable;

public abstract class BrisaAbstractEventSubscription extends Observable {

	protected String sid;
	protected ArrayList<String> callbackUrls;
	protected int lastSeq;
	protected boolean firstMessageSent;
	protected int timeout;
	protected Date date;

	/**
	 * 
	 * @param sid
	 * @param callbackUrls
	 * @param timeout
	 */
	public BrisaAbstractEventSubscription(String sid, ArrayList<String> callbackUrls,
			int timeout) {

		this.sid = sid;
		this.callbackUrls = callbackUrls;
		this.lastSeq = 0;
		this.firstMessageSent = false;
		this.timeout = timeout;
		this.date = new Date(System.currentTimeMillis());
	}

	public int getTimeout(){
		return timeout;
	}
	
	public void renew(int newTimeout) {
		this.date = new Date(System.currentTimeMillis());
		this.timeout = newTimeout;
	}

	public boolean hasExpired() {

		Calendar cal = new GregorianCalendar();
		cal.setTime(date);
		return (timeout < 0) ? false : (System.currentTimeMillis() - cal
				.getTimeInMillis()) >= this.timeout * 1000;
	}

	public int getNextSeq() {
		if (!firstMessageSent) {
			firstMessageSent = true;
			return 0;
		}
		if (this.lastSeq == 0xFFFFFFFF) /* 4294967295L */
			this.lastSeq = 0;

		return ++(this.lastSeq);
	}

	public String getSid() {
		return this.sid;
	}

	public ArrayList<String> getCallbackUrls() {
		return this.callbackUrls;
	}

	/**
	 * 
	 * @return
	 */
	protected URL getUrl() {
		
		String callback = "";
		if (!callbackUrls.isEmpty())
			callback = callbackUrls.get(0);

		URL url;
		try {
			url = new URL(callback);
		} catch (MalformedURLException e) {
			e.printStackTrace();
			return null;
		}
		return url;
	}
}
