package jbrisa.upnp.device.model;

import java.lang.reflect.Type;
import java.net.URI;
import java.sql.Time;
import java.util.ArrayList;
import java.util.Date;
import java.util.Observable;
import java.util.UUID;

public class BrisaStateVariable extends Observable {
	
	private String name;
	private String defaultValue;
	private String maximum;
	private String minimum;
	private String step;
	private String dataType;
	private String strSendEvents;
	private String strMulticast;
	
	private ArrayList<String> allowedValueList;
	
	private boolean sendEvents;
	private boolean multicast;
	
	private String value;

	private Integer nextMulticastSeq;
	//private String multicastUsn;
	//private String multicastUdn;
	//private String multicastSvcid;	

	public BrisaStateVariable(boolean sendEvents, String name, String dataType,
			String defaultValue, String maximum, String minimum, String step,
			boolean multicast) {
		this.sendEvents = sendEvents;
		this.name = name;
		this.dataType = dataType;
		this.defaultValue = defaultValue;
		this.maximum = maximum;
		this.minimum = minimum;
		this.step = step;
		this.multicast = multicast;
		
		this.allowedValueList = new ArrayList<String>();
		
		this.nextMulticastSeq = 1;
	}
	
	public BrisaStateVariable() {
		this(false, "", "", "", "", "", "", false);
	}

	public boolean validadeNewValue(Object newValue) {
		//TODO Validar valor
		return true;
	}
	
	
	public void setDataType(String dataType) {
		this.dataType = dataType;
	}

	public String getStrDataType() {
		return dataType;
	}
	
	/**
	 * 
	 * @return
	 */
	public Type getDataType() {
		
		if (this.dataType == "ui1") {
	        return Integer.TYPE;
	    } else if (this.dataType == "ui2") {
	    	return Integer.TYPE;
	    } else if (this.dataType == "ui4") {
	    	return Integer.TYPE;
	    } else if (this.dataType == "i1") {
	    	return int.class;
	    } else if (this.dataType == "i2") {
	    	return Integer.TYPE;
	    } else if (this.dataType == "i4") {
	    	return Integer.TYPE;
	    } else if (this.dataType == "int") {
	    	return Integer.TYPE;
	    } else if (this.dataType == "r4") {
	        return Double.TYPE;
	    } else if (this.dataType == "r8") {
	    	return Double.TYPE;
	    } else if (this.dataType == "number") {
	    	return Double.TYPE;
	    } else if (this.dataType == "fixed") {
	    	return Double.TYPE;
	    } else if (this.dataType == "float") {
	    	return Double.TYPE;
	    } else if (this.dataType == "char") {
	        return Character.TYPE;
	    } else if (this.dataType == "string") {
	        return String.class;
	    } else if (this.dataType == "date") {
	        return Date.class;
	    } else if (this.dataType == "dateTime") {
	        return Date.class;
	    } else if (this.dataType == "dateTime.tz") {
	    	return Date.class;
	    } else if (this.dataType == "time") {
	        return Time.class;
	    } else if (this.dataType == "time.tz") {
	        return Time.class;
	    } else if (this.dataType == "boolean") {
	        return Boolean.TYPE;
	    } else if (this.dataType == "bin.base64") {
	        return Byte.TYPE;
	    } else if (this.dataType == "bin.hex") {
	        return Byte.TYPE;
	    } else if (this.dataType == "uri") {
	        return URI.class;
	    } else if (this.dataType == "uuid") {
	        return UUID.class;
	    } else {
	        return null;
	    }
		
	}
	
	public void setNextMulticastSeq(Integer nextMulticastSeq) {
		this.nextMulticastSeq = nextMulticastSeq;
	}

	/**
	 * O proximo contador do multicast
	 * @return
	 */
	public Integer getNextMulticastSeq() {
		
		if (this.nextMulticastSeq == Integer.MAX_VALUE) 
			this.nextMulticastSeq = 0;

		return this.nextMulticastSeq++;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public String getDefaultValue() {
		return defaultValue;
	}

	public void setDefaultValue(String defaultValue) {
		this.defaultValue = defaultValue;
	}

	///////////////////////////////////////////////
	public String getMaximum() {
		return maximum;
	}

	public void setMaximum(String maximum) {
		this.maximum = maximum;
	}

	public String getMinimum() {
		return minimum;
	}

	public void setMinimum(String minimum) {
		this.minimum = minimum;
	}

	public String getStep() {
		return step;
	}

	public void setStep(String step) {
		this.step = step;
	}
	
	///////////////////////////////////////////////
	
	public ArrayList<String> getAllowedValueList() {
		return allowedValueList;
	}

	public void setAllowedValueList(ArrayList<String> allowedValueList) {
		this.allowedValueList = allowedValueList;
	}
	
	////////////////////////////////////////////////

//	public void setMulticastUsn(String multicastUsn) {
//		this.multicastUsn = multicastUsn;
//	}
//
//
//	public String getMulticastUdn() {
//		return multicastUdn;
//	}
//
//	public void setMulticastUdn(String multicastUdn) {
//		this.multicastUdn = multicastUdn;
//	}
//
//	public String getMulticastSvcid() {
//		return multicastSvcid;
//	}
//
//	public void setMulticastSvcid(String multicastSvcid) {
//		this.multicastSvcid = multicastSvcid;
//	}

	public String getValue() {
		return value;
	}
	
	//FIXME Validar o value
	public void setValue(String newValue) {
		
		if(this.value != newValue) {
			this.value = newValue;
			if(this.multicastEvents()) {
				setChanged();
				notifyObservers();
			}
		}
	}


///////////////////////////////////////////////
	public String getStrSendEvents() {
		return strSendEvents;
	}
	
	public boolean isSendEvents() {
		return sendEvents;
	}

	public void setSendEvents(boolean sendEvents) {
		this.sendEvents = sendEvents;
		if(this.sendEvents) {
			this.strSendEvents = "yes";
		} else {
			this.strSendEvents = "no";
		}
	}
	
	public void setStrSendEvents(String strSendEvents) {
		if(strSendEvents == null)
			return;
		this.strSendEvents = strSendEvents;
		if(this.strSendEvents.equalsIgnoreCase("yes")) {
			this.sendEvents = true;
		} else {
			this.sendEvents = false;
		}
	}

	///////////////////////////////////////////////////////
	public boolean multicastEvents() {
		return multicast;
	}
	
	public boolean unicastEvents(){
		return sendEvents;
	}

	public void setMulticast(boolean multicast) {
		this.multicast = multicast;
		if(this.multicast) {
			this.strMulticast = "yes";
		} else {
			this.strMulticast = "no";
		}
	}

	/**
	 * 
	 * @param strMulticast
	 */
	public void setStrMulticast(String strMulticast) {
		if(strMulticast == null)
			return;
		this.strMulticast = strMulticast;
		if(this.strMulticast.equalsIgnoreCase("yes")) {
			this.multicast = true;
		} else {
			this.multicast = false;
		}
	}

	public String getStrMulticast() {
		return strMulticast;
	}
	
	
}
