/* $Id$
 *
 * Copyright 2007-2008 Cisco Systems Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy
 * of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */

package org.apache.etch.examples.example;

import org.apache.etch.bindings.java.support.ServerFactory;
import org.apache.etch.examples.example.ExampleHelper;
import org.apache.etch.examples.example.ExampleServer;
import org.apache.etch.examples.example.RemoteExampleClient;
import org.apache.etch.util.core.io.Transport;



/**
 * Main program for ExampleServer. This program makes a listener to accept
 * connections from MainExampleClient.
 */
public class MainExampleListener implements ExampleHelper.ExampleServerFactory
{
	/**
	 * Main program for ExampleServer.
	 * 
	 * @param args command line arguments.
	 * @throws Exception
	 */
	public static void main( String[] args ) throws Exception
	{
		// TODO Change to correct URI
		String uri = "tcp://0.0.0.0:4002";
		
		Transport<ServerFactory> listener = ExampleHelper.newListener( uri, null,
			new MainExampleListener() );

		// Start the Listener
		listener.transportControl( Transport.START_AND_WAIT_UP, 4000 );
	}

	public ExampleServer newExampleServer( RemoteExampleClient client )
	{
		return new ImplExampleServer( client );
	}
}