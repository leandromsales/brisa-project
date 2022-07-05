package jbrisa.upnp.controlpoint.model;

import java.util.ArrayList;

public class BrisaCPAction {
	
	public String name;
	public ArrayList<BrisaCPArgument> argumentList;
	
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public ArrayList<BrisaCPArgument> getArgumentList() {
		return argumentList;
	}
	public void setArgumentList(ArrayList<BrisaCPArgument> argumentList) {
		this.argumentList = argumentList;
	}

	
	
}
