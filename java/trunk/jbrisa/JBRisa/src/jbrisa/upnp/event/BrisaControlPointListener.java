package jbrisa.upnp.event;
import java.util.ArrayList;
import java.util.EventListener;

import jbrisa.upnp.controlpoint.model.BrisaCPActionFault;
import jbrisa.upnp.controlpoint.model.BrisaCPArgumentValue;
import jbrisa.upnp.controlpoint.model.BrisaCPDevice;
import jbrisa.upnp.controlpoint.model.BrisaCPMulticastMessage;
import jbrisa.upnp.controlpoint.model.BrisaCPSubscriptionAnswer;
import jbrisa.upnp.controlpoint.model.BrisaCPUnicastMessage;



public interface BrisaControlPointListener extends EventListener {
	
	//device's change
	void onNewDevice(BrisaCPDevice device);
	
	void onRemovedDevice(BrisaCPDevice device);
	
	void onUpdatedDevice(BrisaCPDevice device);
	
	//actions
	void onActionResult(ArrayList<BrisaCPArgumentValue> e);
	
	void onActionFail(BrisaCPActionFault e);

	//events
	void onMulticastEventRecieved(BrisaCPMulticastMessage e);
	
	void onUnicastEventRecieved(BrisaCPUnicastMessage e);
	
	void onEventSubscriptionAnswer(BrisaCPSubscriptionAnswer e);
	
	//nao funcionando
	//void netConnectionFail(BrisaControlPointEvent evento);


}
