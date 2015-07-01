package jbrisa.utils;

import java.util.logging.Level;
import java.util.logging.Logger;

public class BrisaLogger {

	public static Logger logger = Logger.getLogger("com.br.ic.upnp");

	// ========================================================================
	// SINGLETON ==============================================================
	// ========================================================================
	
	private static BrisaLogger instance = new BrisaLogger();

	private BrisaLogger() {
	}

	public static BrisaLogger getInstance() {
		return instance;
	}
	
	public static void config(String log){
		logger.config(log);
	}
	
	public static void fine(String log){
		logger.fine(log);
	}
	
	public static void finer(String log){
		logger.finer(log);
	}
	
	public static void finest(String log){
		logger.finest(log);
	}
	
	public static void info(String log){
		logger.info(log);
	}
	
	public static void severe(String log){
		logger.severe(log);
	}
	
	public static void warning(String log){
		logger.warning(log);
	}
	
	public static void setLevel(Level l){
		logger.setLevel(l);
	}
	
}
