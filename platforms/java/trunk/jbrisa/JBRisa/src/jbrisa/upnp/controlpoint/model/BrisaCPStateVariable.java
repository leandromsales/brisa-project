package jbrisa.upnp.controlpoint.model;

import java.util.ArrayList;

public class BrisaCPStateVariable {
	
	public String name;
	public String dataType;
	public BrisaCPAllowedValueRange allowedValueRange; 
	public ArrayList<String> allowedValueList;
	public String defaultValue;
	public String value;
	
	public String sendEvents = "no";
	public String multicast = "no";
}
