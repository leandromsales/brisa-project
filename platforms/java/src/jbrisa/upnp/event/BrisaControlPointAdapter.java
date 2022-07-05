package jbrisa.upnp.event;

import java.util.ArrayList;

import jbrisa.upnp.controlpoint.model.BrisaCPActionFault;
import jbrisa.upnp.controlpoint.model.BrisaCPArgumentValue;
import jbrisa.upnp.controlpoint.model.BrisaCPDevice;
import jbrisa.upnp.controlpoint.model.BrisaCPMulticastMessage;
import jbrisa.upnp.controlpoint.model.BrisaCPSubscriptionAnswer;
import jbrisa.upnp.controlpoint.model.BrisaCPUnicastMessage;


public class BrisaControlPointAdapter implements BrisaControlPointListener {

	@Override
	public void onNewDevice(BrisaCPDevice device) {
	}

	@Override
	public void onRemovedDevice(BrisaCPDevice device) {
	}
	
	@Override
	public void onUpdatedDevice(BrisaCPDevice device) {
	}

	@Override
	public void onActionResult(ArrayList<BrisaCPArgumentValue> e) {
	}

	@Override
	public void onActionFail(BrisaCPActionFault e) {
	}
	
	@Override
	public void onMulticastEventRecieved(BrisaCPMulticastMessage e) {
	}

	@Override
	public void onEventSubscriptionAnswer(BrisaCPSubscriptionAnswer e) {
	}

	@Override
	public void onUnicastEventRecieved(BrisaCPUnicastMessage e) {
	}
	
	/*
	@Override
	public void netConnectionFail(BrisaControlPointEvent evento) {
		// TODO Auto-generated method stub
	}
	*/
}
