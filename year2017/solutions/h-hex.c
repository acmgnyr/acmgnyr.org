#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#define EPS .00001

double ax, ay, bx, by, cx, cy;
double ccenx, cceny, crad;
double aidirx, aidiry, bidirx, bidiry, cidirx, cidiry;
double px, py, nx, ny, mx, my;
double pnperpx, pnperpy, nmperpx, nmperpy, mpperpx, mpperpy;
double ex, ey, fx, fy, gx, gy, hx, hy, jx, jy, kx, ky;
double efdist, fgdist, ghdist, hjdist, jkdist, kedist;

/*
 * center of circum cicle is the intersection of the perpendicular bisectors
 * of the sides. perpendicular bisector of AB is x=bx/2
 * perp bisector of AC goes thru (cx/2,cy/2) with direction <-cy, cx>
 * eqn: x= cx/2 - t*cy, y= cy/2 + t*cx: solving cx/2 - t*cy = bx/2 so t = (cx/2 - bx/2)/cy
 * (if cy == 0, not a triangle)
 */
int find_circum_circle()
{
	double t;
	if(fabs(cy) < EPS) return -1;
	t = 0.5*(cx - bx)/cy;
	ccenx = 0.5*bx;
	cceny = 0.5*cy + t*cx;
	crad = sqrt(ccenx*ccenx + cceny*cceny);
	return 0;
}

/*
 * in center is the intersection of the angle bisectors of the angles of the triangle
 * so line from A thru I is just the angle bisector of angle A etc
 * to find the angle bisector line we find a ppoint on each adjacent side equal
 * disatance from the vertex. The the angle bisector goes thru the midpoint of the line
 * between those 2 points
 * each line intersects the circum circle (x - ccenx)^2 + (y - cceny)^2 = crad^2
 * in two points but one them is th triangle vertex so if we use a parametric equation
 * for the line based at the vertex one of the solutions from plugging the parametric
 * line equation (x = vertx + t*dirx, y = verty + t*diry) into the circle equation
 * is that t= 0 will be a root
 * (vertx + t*dirx -ccenx)^2 + (verty - t*diry -cceny)^2 =
 * vertx^2 + t^2*dirx^2 +ccenx^2  + 2*t*vertx*dirx -2*vertx*ccenx - 2*t*dirx*ccenx +
 * verty^2 + t^2*diry^2 +cceny^2  + 2*t*verty*diry -2*verty*cceny - 2*t*diry*cceny =
 * (vertx^2-2*vertx*ccenx+ccenx^2 + verty^2-2*verty*cceny+cceny^2) +
 * t^2*(dirx^2 + diry^2) + 2t*(vertx*dirx-dirx*ccenx+verty*diry-diry*cceny = crad^2
 * the first term in parens = crad^2 since (vertx, verty) is on the circum circle
 * factoring out t we get
 * t*(dirx^2+diry^2) = 2*(dirx*(ccenx-vertx) + diry*(cceny-verty)
 * if we make (dirx^2+diry^2) = 1, t just = the right side
 */
int find_pnm()
{
	double d, d1, t, tx, ty, ux, uy;
	// a bisector
	d = sqrt(cx*cx + cy*cy);
	tx = cx; ty = cy; ux = d; uy = 0;
	aidirx = 0.5*(tx + ux); aidiry = 0.5*(ty + uy);
	d = sqrt(aidirx*aidirx + aidiry*aidiry);
	aidirx /= d;
	aidiry /= d;
	t = 2.0*(aidirx*(ccenx) + aidiry*(cceny));
	mx = t*aidirx; my = t*aidiry;

	// b bisector
	d = sqrt((cx - bx)*(cx - bx) + cy*cy);
	tx = cx; ty = cy; ux = bx - d; uy = 0;
	bidirx = 0.5*(tx + ux) - bx; bidiry = 0.5*(ty + uy) - by;
	d = sqrt(bidirx*bidirx + bidiry*bidiry);
	bidirx /= d;
	bidiry /= d;
	t = 2.0*(bidirx*(ccenx - bx) + bidiry*(cceny));
	nx = t*bidirx + bx; ny = t*bidiry;

	// c bisector
	d = sqrt((cx - bx)*(cx - bx) + cy*cy);
	d1 = sqrt(cx*cx + cy*cy);
	tx = bx; ty = by; ux = cx*(1.0 - d/d1); uy = cy*(1.0 - d/d1);
	cidirx = 0.5*(tx + ux) - cx; cidiry = 0.5*(ty + uy) - cy;
	d = sqrt(cidirx*cidirx + cidiry*cidiry);
	cidirx /= d;
	cidiry /= d;
	t = 2.0*(cidirx*(ccenx - cx) + cidiry*(cceny - cy));
	px = t*cidirx + cx; py = t*cidiry + cy;

	return 0;
}

/*
 * we get the equations of PN, NM, MP as a*(x-ptx) + b*(y-pty) = 0
 * where <a,b> is a vector perpendicular to the line and (ptx,pty) is one
 * of the endpoints;
 * we then get the parametrix equations of the sides x = vertx + t*dirx, y = verty + t*diry
 * sustitute in
 * a*(vertx + t*dirx - ptx) + b*(verty + t*diry - pty) = 0
 * so t*(a*dirx + b*diry) = a*(ptx - vertx) + b*(pty - verty)
 * solve for t and plug into the side equation to get the intersection
 */
int find_efghjk()
{
	double t, num, denom, diracx, diracy, dirabx, diraby, dirbcx, dirbcy;
	diracx = cx; diracy = cy;
	dirabx = bx; diraby = 0;
	dirbcx = cx - bx; dirbcy = cy;
	// PN hitting AB and AC
	pnperpx = (py - ny); pnperpy = (nx - px);
	denom = pnperpx*dirabx + pnperpy*diraby;
	num = pnperpx*(px - 0) + pnperpy*(py - 0);
	t = num/denom;
	ex = 0 + t*dirabx;
	ey = 0 + t*diraby;
	denom = pnperpx*diracx + pnperpy*diracy;
	num = pnperpx*(px - 0) + pnperpy*(py - 0);
	t = num/denom;
	fx = 0 + t*diracx;
	fy = 0 + t*diracy;

	// NM hitting AC and BC
	nmperpx = (ny - my); nmperpy = (mx - nx);
	denom = nmperpx*diracx + nmperpy*diracy;
	num = nmperpx*(nx - 0) + nmperpy*(ny - 0);
	t = num/denom;
	gx = 0 + t*diracx;
	gy = 0 + t*diracy;
	denom = nmperpx*dirbcx + nmperpy*dirbcy;
	num = nmperpx*(nx - bx) + nmperpy*(ny - by);
	t = num/denom;
	hx = bx + t*dirbcx;
	hy = 0 + t*dirbcy;

	// MP hitting BC and AB
	mpperpx = (py - my); mpperpy = (mx - px);
	denom = mpperpx*dirbcx + mpperpy*dirbcy;
	num = mpperpx*(mx - bx) + mpperpy*(my - 0);
	t = num/denom;
	jx = bx + t*dirbcx;
	jy = 0 + t*dirbcy;
	denom = mpperpx*dirabx + mpperpy*diraby;
	num = mpperpx*(px - 0) + mpperpy*(py - 0);
	t = num/denom;
	kx = 0 + t*dirabx;
	ky = 0 + t*diraby;

	return 0;
}

int find_dists()
{
	efdist = sqrt((ex-fx)*(ex-fx) + (ey-fy)*(ey-fy));
	fgdist = sqrt((gx-fx)*(gx-fx) + (gy-fy)*(gy-fy));
	ghdist = sqrt((hx-gx)*(hx-gx) + (hy-gy)*(hy-gy)); 
	hjdist = sqrt((hx-jx)*(hx-jx) + (hy-jy)*(hy-jy));
	jkdist = sqrt((kx-jx)*(kx-jx) + (ky-jy)*(ky-jy)); 
	kedist = sqrt((ex-kx)*(ex-kx) + (ey-ky)*(ey-ky));
	return 0;
}

char inbuf[256];

int main()
{
	int nprob, curprob, index;
	if(fgets(&(inbuf[0]), 255, stdin) == NULL)
	{
		fprintf(stderr, "Read failed on problem count\n");
		return -1;
	}
	if(sscanf(&(inbuf[0]), "%d", &nprob) != 1)
	{
		fprintf(stderr, "Scan failed on problem count\n");
		return -2;
	}
	for(curprob = 1; curprob <= nprob ; curprob++)
	{
		if(fgets(&(inbuf[0]), 255, stdin) == NULL)
		{
			fprintf(stderr, "Read failed on problem %d header\n", curprob);
			return -3;
		}
		// get prob num and coordinates
		if(sscanf(&(inbuf[0]), "%d %lf %lf %lf", &index, &bx, &cx, &cy) != 4)
		{
			fprintf(stderr, "scan failed on problem header problem index %d\n",
				curprob);
			return -6;
		}
		if(index != curprob)
		{
			fprintf(stderr, "problem index %d not = expected problem %d\n",
				index, curprob);
			return -7;
		}
		ax = ay = by = 0.0;
		if(find_circum_circle() != 0) {
			fprintf(stderr, "problem index %d not not a triangle\n",
				index);
			return -8;
		}
		find_pnm();
		find_efghjk();
		find_dists();
		printf("%d %.4lf %.4lf %.4lf %.4lf %.4lf %.4lf\n",
			index, efdist, fgdist, ghdist, hjdist, jkdist, kedist);
	}
	return 0;
}

