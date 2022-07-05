package jbrisa.upnp.controlpoint.model;

public class BrisaCPArgument {
	
	public String name;
	public String direction;
	public String relatedStateVariable;
	public String retval;
	public String value;
	
	public static String ARGUMENT_DIRECTION_IN = "in";
	public static String ARGUMENT_DIRECTION_OUT = "out";
	
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public String getDirection() {
		return direction;
	}
	public void setDirection(String direction) {
		this.direction = direction;
	}
	public String getRelatedStateVariable() {
		return relatedStateVariable;
	}
	public void setRelatedStateVariable(String relatedStateVariable) {
		this.relatedStateVariable = relatedStateVariable;
	}
	public String getRetval() {
		return retval;
	}
	public void setRetval(String retval) {
		this.retval = retval;
	}
	public String getValue() {
		return value;
	}
	public void setValue(String value) {
		this.value = value;
	}

	
}
