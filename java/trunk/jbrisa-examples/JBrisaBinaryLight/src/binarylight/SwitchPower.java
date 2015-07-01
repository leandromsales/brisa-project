package binarylight;

import java.util.ArrayList;

import upnp.device.core.BrisaService;
import upnp.device.model.BrisaArgument;

public class SwitchPower extends BrisaService {
	
	private final static String SERVICE_ID = "SwitchPower";
	private static final String SERVICE_TYPE = "urn:schemas-upnp-org:service:SwitchPower:1";
	//private static final String SERVICE_XML_PATH = "/SwitchPower-scpd.xml";
	//private static final String SERVICE_EVENT_SUB = "/SwitchPower/eventSub";
	//private static final String SERVICE_CONTROL = "/SwitchPower/control";

	public SwitchPower() {
		super(SERVICE_ID, SERVICE_TYPE);
	}
	
	/**
	 * Retorna o Status da Lampada Binaria
	 * 
	 * @param in Argumentos de entrada
	 * @return Argumentos de saida
	 */
	public ArrayList<BrisaArgument> getStatus(ArrayList<BrisaArgument> inArgs) {
		
		System.out.println("getStatus");
		
		BrisaArgument out = new BrisaArgument("ResultStatus", "out", "Status");	
		out.setValue(this.getStateVariable(out.getRelatedStateVariable()).getValue());
		
		ArrayList<BrisaArgument> outArgs = new ArrayList<BrisaArgument>();
		outArgs.add(out);
		
		System.out.println("in Args: " + inArgs);
		System.out.println("out Args: " + outArgs);
		
	    return outArgs;
	}

	/**
	 * Retorna o Status da Lampada Binaria
	 * 
	 * @param in Argumentos de entrada
	 * @return Argumentos de saida
	 */
	public ArrayList<BrisaArgument> getTarget(ArrayList<BrisaArgument> inArgs)
	{
		System.out.println("getTarget");
	    		
		BrisaArgument out = new BrisaArgument("RetTargetValue", "out", "Target");
		out.setValue(this.getStateVariable(out.getRelatedStateVariable()).getValue());

		ArrayList<BrisaArgument> outArgs = new ArrayList<BrisaArgument>();
		outArgs.add(out);
		
		System.out.println("in Args: " + inArgs);
		System.out.println("out Args: " + outArgs);
		
	    return outArgs;
	}

	/**
	 * Muda o estado da lampada binaria
	 * 
	 * @param in Argumentos de entrada
	 * @return Argumentos de saida (vazio)
	 */
	public ArrayList<BrisaArgument> setTarget(ArrayList<BrisaArgument> inArgs)
	{
		System.out.println("setTarget");
		
		BrisaArgument in = inArgs.get(0);
		this.getStateVariable("Target").setValue(in.getValue());
		this.getStateVariable("Status").setValue(in.getValue());
		
		System.out.println("in Args: " + inArgs);
		System.out.println("out Args: <vazio>");
		
	    return new ArrayList<BrisaArgument>(); //Sem out Arguments
	}
	
	

}
