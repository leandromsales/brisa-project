package jbrisa.upnp.device.event;

import java.util.ArrayList;

import jbrisa.upnp.controlpoint.event.BrisaAbstractEventSubscription;


public class BrisaEventSubscription extends BrisaAbstractEventSubscription {

	public BrisaEventSubscription(String sid, ArrayList<String> callbackUrls,
			int timeout) {
		super(sid, callbackUrls, timeout);
		// TODO Auto-generated constructor stub
	}

}
