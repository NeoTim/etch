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

/**
 * Description of IntRangeConstraint
 */
public class IntRangeConstraint implements Constraint
{
	/**
	 * Constructs the IntRangeConstraint.
	 *
	 * @param min the minimum value allowed.
	 * 
	 * @param max the maximum value allowed.
	 */
	public IntRangeConstraint( int min, int max )
	{
		if (min > max)
			throw new IllegalArgumentException( "min > max" );
		this.min = min;
		this.max = max;
	}
	
	private final int min;
	
	private final int max;
	
	public void checkValue( Object value ) throws Exception
	{
		int v = ((Integer) value).intValue();
		if (v < min || v > max)
			throw new Exception( "value not in range "+min+" to "+max );
	}
	
	@Override
	public String toString()
	{
		return "value must be in range "+min+" to "+max;
	}
}
