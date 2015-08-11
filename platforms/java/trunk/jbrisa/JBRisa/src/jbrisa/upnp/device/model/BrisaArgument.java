package jbrisa.upnp.device.model;

public class BrisaArgument {

	//TODO Implementar BrisaArgument
	
    private String name;
    private String direction;
    private String relatedStateVariable;
    
    private String value;
    
	public BrisaArgument(String name, String direction,
			String relatedStateVariable) {
		this.name = name;
		this.direction = direction;
		this.relatedStateVariable = relatedStateVariable;
	}
	
	public BrisaArgument() {
		this("", "", "");
	}
	
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

	public void setValue(String valor) {
		this.value = valor;
	}

	public String getValue() {
		return value;
	}
    
    
    
}
