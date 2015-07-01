package jbrisa.upnp.device.model;

import java.lang.reflect.Method;
import java.util.ArrayList;

import jbrisa.upnp.device.core.BrisaService;



/**
 * class Brisa::BrisaAction brisaaction.h BrisaUpnp/BrisaAction
 * 
 * \brief Template method class that represents each service's action.
 * 
 * If any of the returned output arguments is not defined in the service
 * description file, BrisaAction will show an error message at the debug output
 * stream and send an error message to the control point.
 * 
 * @author wendell
 * 
 */
public class BrisaAction {

	private ArrayList<BrisaArgument> argumentList;
	private String name;
	private BrisaService service;
	
	private Method method;

	public BrisaAction(String name, BrisaService service) {
		this.name = name;
		this.service = service;
	}
	
	public BrisaAction(String name) {
		this(name, null);
	}
	
	public BrisaAction() {
		this("", null);
	}

	
	/////////////////////////
	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public BrisaService getService() {
		return service;
	}

	public void setService(BrisaService service) {
		this.service = service;
	}

	public void setArgumentList(ArrayList<BrisaArgument> argumentList) {
		this.argumentList = argumentList;
	}

	/**
	 * Returns this action's list of arguments.
	 */
	public ArrayList<BrisaArgument> getArgumentList() {
		return argumentList;
	}

	/**
	 * Adds an argument with given \a name, \a direction and \a
	 * relatedStateVariable to this action's list of arguments.
	 */
	public void addArgument(String name, String direction, String relatedStateVariable) {
		BrisaArgument arg = new BrisaArgument(name, direction, relatedStateVariable);
		this.argumentList.add(arg);
	}

	/**
	 * Adds given \a argument to this action's list of arguments.
	 */
	public void addArgument(BrisaArgument argument) {
		this.argumentList.add(argument);
	}

	/**
	 * Adds given list of \a arguments to this action's list of arguments.
	 */
	public void addArguments(ArrayList<BrisaArgument> arguments) {
		this.argumentList.addAll(arguments);
	}

	/**
	 * Clears this action's argument list.
	 */
	public void clearArgumentList() {
		argumentList.clear();
	}

	/**
	 * Remove given \a argument from this action's list of arguments.
	 */
	public boolean removeArgument(BrisaArgument argument) {
		return this.argumentList.remove(argument);
	}

	/*
	 * ! Validates \a inArguments, \a outArguments and runs the action. \a
	 * outArguments is an output parameter. This method returns true in case of
	 * successful running of the action, else returns false.
	 */
	public boolean call(BrisaArgument inArguments, BrisaArgument outArguments) {
		//TODO implementar call
		return false;
	}

	/**
	 * Returns the related service's state variable with the given \a name. If
	 * it cannot find its related service or the state variable, then it returns
	 * a null pointer.
	 */
	public BrisaStateVariable getStateVariable(String name) 	{
	    if (this.service == null) {
	        System.err.println(this.getName() + " action couldn't find its related Service.");
	        return null;
	    }

	    BrisaStateVariable stateVariable = service.getStateVariable(name);
	    if (stateVariable == null) {
	        System.err.println(this.getName()+ " action couldn't find the State Variable.");
	    }

	    return stateVariable;
	}
	
	/**
	 * Sets the method to be invoked when the action is executed.
	 */
	public void setMethod(Method method, BrisaService service) {
		this.method = method;
		this.service = service;
	}

	/**
	 * ! Returns the method to be invoked when the action is executed.
	 */
	public Method getMethod() {
		return this.method;
	}

	/**
	 * Returns the related input argument with the given \a name.
	 */
	public BrisaArgument getInArgument(String name) {
		return getArgument(name, "in");
	}

	/**
	 * Returns the related output argument with the given \a name.
	 */
	public BrisaArgument getOutArgument(String name) {
		return getArgument(name, "out");
	}

	/**
	 * Returns the related argument with the given \a name and \a direction.
	 */
	private BrisaArgument getArgument(String name, String direction) {
		for(BrisaArgument a: argumentList) {
			if(a.getName().equalsIgnoreCase(name) && a.getDirection().equalsIgnoreCase(direction)) {
				return a;
			}
		}
		return null;
	}

}
