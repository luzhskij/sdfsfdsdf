/*
 * This file is part of the Geoid and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef CONSTS
#define CONSTS
const double pi=3.14159265358979323846;
const double undef=1.70141e+37;
#endif //CONSTS

#ifndef TYPES
#define TYPES
typedef unsigned int UINT;
#endif //TYPES

#ifndef FUNCS
#define FUNCS

inline void MinMax ( double d, double& Min, double& Max )
{
	if ( d > Max )
		Max = d;
	else if ( d < Min )
		Min = d;
}

inline void MinMax ( float d, double& Min, double& Max )
{
	if ( d > Max )
		Max = d;
	else if ( d < Min )
		Min = d;
}

inline void swap ( double& _a, double& _b )
{
	double buf;
	buf = _a;
	_a = _b;
	_b = buf;
}

/*
inline int round (double _v)
{
	if ((_v-floor(_v))<0.5)
		return floor(_v);
	else
		return ceil(_v);
}
*/
#endif //FUNCS
