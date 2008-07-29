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

package etch.util.cmd;

import java.util.Iterator;
import java.util.NoSuchElementException;

/**
 * Simply an Iterator for String[].
 */
public class StringArrayIterator implements Iterator<String>
{
	/**
	 * @param array
	 */
	public StringArrayIterator( String[] array )
	{
		this.array = array;
	}
	
	private final String[] array;
	
	private int index;
	
	/* (non-Javadoc)
	 * @see java.util.Iterator#hasNext()
	 */
	public boolean hasNext()
	{
		return index < array.length;
	}

	/* (non-Javadoc)
	 * @see java.util.Iterator#next()
	 */
	public String next()
	{
		if (index >= array.length)
			throw new NoSuchElementException();
		
		return array[index++];
	}

	/* (non-Javadoc)
	 * @see java.util.Iterator#remove()
	 */
	public void remove()
	{
		throw new UnsupportedOperationException();
	}
}
