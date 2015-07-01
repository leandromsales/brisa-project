package jbrisa.utils;

import java.util.UUID;

public class UUIDFactory {

	/**
	 * Returns a random UUID
	 * @return UUID
	 */
	public static String getUuid() {
		return UUID.randomUUID().toString();
	}
	
	/**
	 * Returns a random complete UUID 
	 * @return A complete UUID
	 */
	public static String getCompleteUiid() {
		return "uuid:" + getUuid();
	}
	
}
