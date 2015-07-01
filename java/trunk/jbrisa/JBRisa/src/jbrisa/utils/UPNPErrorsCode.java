package jbrisa.utils;

public enum UPNPErrorsCode {

	UPNP_SUCCESS(0, ""), 
	UPNP_INVALID_ACTION(401, "Invalid Action"), 
	UPNP_INVALID_ARGUMENTS(402, "Invalid Arguments"), 
	UPNP_ACTION_FAILED(501, "Action Failed"), 
	UPNP_INVALID_VALUE_ARGUMENT(600, "Invalid Value Argument"), 
	UPNP_ARGUMENT_VALUE_OUT_OF_RANGE(601, "Argument Value Out of Range"), 
	UPNP_OPTIONAL_ACTION_NOT_IMPLEMENTED(602, "Optional Action not Implemented"), 
	UPNP_OUT_OF_MEMORY(603, "Out of Memory"), 
	UPNP_HUMAN_INTERVENTION_REQUIRED(604, "Human Intervention Required"), 
	UPNP_STRING_ARGUMENT_TOO_LONG(605, "String Argument Too Long");

	private Integer value;
	private String desc;

	/**
	 * 
	 * @param value
	 */
	private UPNPErrorsCode(Integer value, String desc) {
		this.value = value;
	}

	/**
	 * Retorna o codigo do erro UPnP
	 * @return Codigo do erro UPnP
	 */
	public double getValue() {
		return value;
	}
	
	/**
	 * Retorna a descricao do erro UPnP
	 * @return Descricao do erro UPnP
	 */
	public String getDescription() {
		return desc;
	}

}
