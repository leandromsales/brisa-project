package jbrisa.upnp.event;
import java.util.ArrayList;
import java.util.Collection;

import jbrisa.upnp.controlpoint.model.BrisaCPActionFault;
import jbrisa.upnp.controlpoint.model.BrisaCPArgumentValue;
import jbrisa.upnp.controlpoint.model.BrisaCPDevice;
import jbrisa.upnp.controlpoint.model.BrisaCPMulticastMessage;
import jbrisa.upnp.controlpoint.model.BrisaCPSubscriptionAnswer;
import jbrisa.upnp.controlpoint.model.BrisaCPUnicastMessage;


public class BrisaControlPointEventManager {

	private Collection<BrisaControlPointListener> brisaListeners = new ArrayList<BrisaControlPointListener>();

	public synchronized void removeListener(BrisaControlPointListener l) {
		brisaListeners.remove(l);
	}

	public synchronized void addListener(BrisaControlPointListener l) {
		if (!brisaListeners.contains(l)) {
			brisaListeners.add(l);
		}
	}

	@SuppressWarnings("unchecked")
	protected void dispatchDeviceAdded(BrisaCPDevice d) {

		Collection<BrisaControlPointListener> ml;

		synchronized (this) {
			ml = (Collection<BrisaControlPointListener>) (((ArrayList<BrisaControlPointListener>) brisaListeners)
					.clone());
		}

		for (BrisaControlPointListener meuListener : ml) {
			meuListener.onNewDevice(d);
		}

	}

	@SuppressWarnings("unchecked")
	protected void dispatchEventDeviceRemoved(BrisaCPDevice d) {

		Collection<BrisaControlPointListener> ml;

		synchronized (this) {
			ml = (Collection<BrisaControlPointListener>) (((ArrayList<BrisaControlPointListener>) brisaListeners)
					.clone());
		}

		for (BrisaControlPointListener meuListener : ml) {
			meuListener.onRemovedDevice(d);
		}

	}
	
	@SuppressWarnings("unchecked")
	protected void dispatchEventDeviceUpdate(BrisaCPDevice d) {

		Collection<BrisaControlPointListener> ml;

		synchronized (this) {
			ml = (Collection<BrisaControlPointListener>) (((ArrayList<BrisaControlPointListener>) brisaListeners)
					.clone());
		}

		for (BrisaControlPointListener meuListener : ml) {
			meuListener.onUpdatedDevice(d);
		}

	}
	
	@SuppressWarnings("unchecked")
	protected void dispatchActionResult(ArrayList<BrisaCPArgumentValue> arrayArg) {

		Collection<BrisaControlPointListener> ml;

		synchronized (this) {
			ml = (Collection<BrisaControlPointListener>) (((ArrayList<BrisaControlPointListener>) brisaListeners)
					.clone());
		}
		
		for (BrisaControlPointListener meuListener : ml) {
			meuListener.onActionResult(arrayArg);
		}
	}
	
	@SuppressWarnings("unchecked")
	protected void dispatchActionFail(BrisaCPActionFault faultAnswer) {

		Collection<BrisaControlPointListener> ml;

		synchronized (this) {
			ml = (Collection<BrisaControlPointListener>) (((ArrayList<BrisaControlPointListener>) brisaListeners)
					.clone());
		}

		for (BrisaControlPointListener meuListener : ml) {
			meuListener.onActionFail(faultAnswer);
		}
	}
	
	@SuppressWarnings("unchecked")
	protected void dispatchMulticastEventRecieved(BrisaCPMulticastMessage arg) {

		Collection<BrisaControlPointListener> ml;

		synchronized (this) {
			ml = (Collection<BrisaControlPointListener>) (((ArrayList<BrisaControlPointListener>) brisaListeners)
					.clone());
		}

		for (BrisaControlPointListener meuListener : ml) {
			meuListener.onMulticastEventRecieved(arg);
		}
	}
	
	@SuppressWarnings("unchecked")
	protected void dispatchUnicastEventRecieved(BrisaCPUnicastMessage arg) {

		Collection<BrisaControlPointListener> ml;

		synchronized (this) {
			ml = (Collection<BrisaControlPointListener>) (((ArrayList<BrisaControlPointListener>) brisaListeners)
					.clone());
		}

		for (BrisaControlPointListener meuListener : ml) {
			meuListener.onUnicastEventRecieved(arg);
		}
	}
	
	/*
	@SuppressWarnings("unchecked")
	protected void dispatchNetConnectionFault() {

		Collection<BrisaControlPointListener> ml;

		synchronized (this) {
			ml = (Collection<BrisaControlPointListener>) (((ArrayList<BrisaControlPointListener>) brisaListeners)
					.clone());
		}

		BrisaControlPointEvent evento = new BrisaControlPointEvent(this);
		
		for (BrisaControlPointListener meuListener : ml) {
			meuListener.netConnectionFail(evento);
		}
	}
	*/
	
	@SuppressWarnings("unchecked")
	protected void dispatchSubscriptionAnswerRecieved(BrisaCPSubscriptionAnswer arg) {

		Collection<BrisaControlPointListener> ml;

		synchronized (this) {
			ml = (Collection<BrisaControlPointListener>) (((ArrayList<BrisaControlPointListener>) brisaListeners)
					.clone());
		}

		for (BrisaControlPointListener meuListener : ml) {
			meuListener.onEventSubscriptionAnswer(arg);
		}
	}

}
