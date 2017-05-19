//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 


package inet.LIMoSim.omnet.mobility;
import inet.LIMoSim.omnet.mobility.strategicModel.IStrategicModel;
import inet.LIMoSim.omnet.mobility.followerModel.IFollowerModel;

import inet.mobility.base.MovingMobilityBase;

module LIMoSimCar extends MovingMobilityBase
{
	parameters:
        volatile double speed @unit(mps) = default(mps); // use uniform(minSpeed, maxSpeed) or another distribution       
    	@class(LIMoSimCar);
    	
    	string map;

    	string strategicModelType = default("Trip");
    	string followerModelType = default("IDM");
    
    	string configuration = default("");
    	string way = default("-1");
    	int segment = default(-1);
    	int lane = default(-1);
    	double offset @unit(m) = default(0m);
    
    submodules:
        strategicModel: <strategicModelType> like IStrategicModel if strategicModelType != "" {}
        followerModel: <followerModelType> like IFollowerModel if followerModelType != "" {}
        
 }
