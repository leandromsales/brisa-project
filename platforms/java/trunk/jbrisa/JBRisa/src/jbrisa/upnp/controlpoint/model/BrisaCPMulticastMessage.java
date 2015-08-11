package jbrisa.upnp.controlpoint.model;

public class BrisaCPMulticastMessage {

	private String usn;
	private String svcid;
	private String seq;
	private String lvl;
	
	private String name;
	private String value;
	
	public String getUsn() {
		return usn;
	}
	public void setUsn(String usn) {
		this.usn = usn;
	}
	public String getSvcid() {
		return svcid;
	}
	public void setSvcid(String svcid) {
		this.svcid = svcid;
	}
	public String getSeq() {
		return seq;
	}
	public void setSeq(String seq) {
		this.seq = seq;
	}
	public String getLvl() {
		return lvl;
	}
	public void setLvl(String lvl) {
		this.lvl = lvl;
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
