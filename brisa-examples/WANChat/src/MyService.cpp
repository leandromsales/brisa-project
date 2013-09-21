/*
 * MyService.cpp
 *
 *  Created on: Sep 21, 2013
 *      Author: rodrigopex
 */
#include "MyService.hpp"

BrisaOutArgument* MyService::actionName1(BrisaInArgument* const inArguments,
		BrisaAction* const action) {
	Q_UNUSED(action);
	BrisaOutArgument *outArgs = new BrisaOutArgument();
	QString inArg = inArguments->value("ExampleInText");
	outArgs->insert("TextOut", inArg + "Out!!");
	return outArgs;
}
BrisaOutArgument* MyService::actionName2(BrisaInArgument* const inArguments,
		BrisaAction* const action) {
	Q_UNUSED(action);
	BrisaOutArgument *outArgs = new BrisaOutArgument();
	QString inArg = inArguments->value("ExampleInText");
	outArgs->insert("TextOut", inArg + "Out!!");
	return outArgs;
}
BrisaOutArgument* MyService::actionName3(BrisaInArgument* const inArguments,
		BrisaAction* const action) {
	Q_UNUSED(action);
	BrisaOutArgument *outArgs = new BrisaOutArgument();
	QString inArg = inArguments->value("ExampleInText");
	outArgs->insert("TextOut", inArg + "Out!!");
	return outArgs;
}

