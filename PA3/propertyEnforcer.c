#include "propertyEnforcer.h"
#include <math.h>

int propertyZero(int emptySpots, int nc0, int nc1, int nc2,char c, char c0, char c1, char c2) {
    if (c == c0 || c == c1 || c == c2) {
        if (c == c0 || c == c1 ) {
            if (emptySpots > nc0 + nc1 - nc2 +1)  {
                return 1;
            }
        }

        else {
        	if (nc0 + nc1 == nc2 && emptySpots <= 1) {
        		return -1;
        	}
            if (nc2 < nc0 + nc1 || emptySpots > nc2) {
                return 1;
            }
        }
    }

    else {
    	if (nc0 + nc1 != nc2 && emptySpots <= abs(nc0+nc1-nc2)) {
    		return -1;
    	}
        if ( (emptySpots > nc0 + nc1 - nc2) || ( nc0 + nc1 == nc2 )) {
            return 1;
        }
    }

    return -1;
}   



int propertyOne(int emptySpots, int num_threads, int seg_size, int nc0, int nc1, int nc2,char c, char c0, char c1, char c2) {
	int tot = nc0 + 2*nc1 - nc2;
	if (c == c0 || c == c1 || c == c2) {
		if (c == c0) {

			if (num_threads == 3 && seg_size > 3) {
				if (emptySpots > tot + 2) {
			  		return 1;
				}
				// if (emptySpots == tot + 2) {
				// 	return 1;
				// }
				if (emptySpots == tot + 1) {
			  		return 1;
				}
				if (tot <= -1 && (tot % 2 != 0)) {
					return 1;
				}
			}
			else {
				if (emptySpots > tot + 1) {
					return 1;
				}
			}
		}
		if (c == c1) {
			if (seg_size %2 == 1 && nc1 == 0 && tot <=-2 ) {
				return 1;
			}
			if (seg_size %2 == 1 && nc1 == 1) {
				return -1;
			}
			if (tot < 0 && tot%2 == 0) {
				return 1;
			}
			if ( (num_threads == 3) && (seg_size == 4 || seg_size == 2)) {
				return -1;
			}
			if (emptySpots > tot + 2) {
			  return 1;
			}
			if (tot <= -2 && (tot % 2 == 0) ) {
				return 1;
			}
		}
		if (c == c2) {

			if (tot > 0 && !(emptySpots == 2 && tot == 0)) {
			  return 1;
			}
			
			if (tot < 0  && emptySpots <= abs(tot) + 1) {
				return -1;
			}
			if (tot <= 0) {
			  if (emptySpots > 1) {
			    return 1;
			  }
			}
		}
	}

	else {
		if (nc0 + 2*nc1 != nc2 && emptySpots <= abs(tot)) {
    		return -1;
    	}
		if (nc0 + 2*nc1 == nc2 || emptySpots > abs(tot) + 2) {
			return  1;
		}
	}

	return -1;
}

int propertyTwo(int emptySpots,int num_threads, int seg_size, int nc0, int nc1, int nc2, char c, char c0, char c1, char c2) {
    

	if (c== c0 || c == c1 || c == c2){
        if (c== c2){
            if ( (nc0 * nc1 != 0) && (nc0 * nc1 - nc2 != 0)) {
                return 1;
            }
        }
        if(c== c0 || c== c1) {
        	if (c == c0) {
        		if (seg_size % 2 == 0 && num_threads == 3) {
        			if (nc1 + nc2 == 0) {
        				return 1;
        			}
        			else {
        				return -1;
        			}
        		}
        		if (emptySpots > (nc0+1)*nc1 - nc2) {
        			return 1;
        		}
        	}

        	else {
        		if (seg_size % 2 == 0 && num_threads == 3) {
        			if (nc0 + nc2 == 0) {
        				return 1;
        			}
        			else {
        				return -1;
        			}
        		}
        		if (emptySpots > nc0*(nc1+1) - nc2) {
        			return 1;
        		}
        	}

        }

        
       
    }
    else{
    	if (nc0 * nc1 == nc2) {
    		return 1;
    	}
    }
    return -1;

}

