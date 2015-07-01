package jbrisa.upnp.controlpoint.model;

public class BrisaCPArgumentValue {
	
	private String name;
	private String value;
	
	public BrisaCPArgumentValue(String name, String value) {
		super();
		this.name = name;
		this.value = value;
	}
	
	public BrisaCPArgumentValue() {
	}
	
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public String getValue() {
		return value;
	}
	public void setValue(String value) {
		this.value = value;
	}

}
