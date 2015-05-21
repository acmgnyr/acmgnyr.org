/*
 * 2005 ACM Greater New York Regional Collegiate Programming Contest
 * Problem D -- Kindergarten Graduation
 */

#include <stdio.h>
#include <string.h>
#include <math.h>

#define MAX_POS			27	// 24 children + space+ one extra on each end
#define MAX_CHILDREN	24
#define CHARS_PER_LINE	50

char children[MAX_POS];
int totalPos, rightGoodBoy, leftGoodGirl, nBadGirls, nBadBoys, spaceLoc;
int altLeft, altRight;	// left and right ends of alt G,B
int maxMoves;

char moves[625];
int nMoves;

int
InitPos(int nGirls, int nBoys)
{
	int i;

	if((nGirls + nBoys) > MAX_CHILDREN)
	{
		return -1;
	}
	// fake boy on the left
	children[0] = 'B';
	rightGoodBoy = 0;
	// put in nGirls
	for(i = 1; i <= nGirls ; i++)
	{
		children[i] = 'G';
	}
	nBadGirls = nGirls;
	children[nGirls+1] = ' ';
	spaceLoc = nGirls+1;
	for(i = spaceLoc+1 ; i <= spaceLoc+nBoys ; i++)
	{
		children[i] = 'B';
	}
	nBadBoys = nBoys;
	// fake good girl
	children[spaceLoc + nBoys + 1] = 'G';
	leftGoodGirl = spaceLoc + nBoys + 1;
	totalPos = nGirls + nBoys + 1;
	maxMoves = nGirls * nBoys + nGirls + nBoys;
	nMoves = 0;
	return 0;
}

void
DumpConfig()
{
	int i;
	printf("rGB %d aL %d aR %d LGG %d\n",
		rightGoodBoy, altLeft, altRight, leftGoodGirl);
	for(i = 1; i <= totalPos ; i++)
	{
		putchar(children[i]);
	}
	putchar('\n');
}

int
SlideLeft()
{
	if(spaceLoc >= totalPos)
	{
		return -1;
	}
	children[spaceLoc] = children[spaceLoc+1];
	spaceLoc++;
	children[spaceLoc] = ' ';
	moves[nMoves++] = 's';
	if(nMoves > maxMoves)
	{
		return -2;
	}
	return 0;
}

int
SlideRight()
{
	if(spaceLoc <= 1)
	{
		return -1;
	}
	children[spaceLoc] = children[spaceLoc-1];
	spaceLoc--;
	children[spaceLoc] = ' ';
	moves[nMoves++] = 'S';
	if(nMoves > maxMoves)
	{
		return -2;
	}
	return 0;
}

int
HopLeft()
{
	if(spaceLoc >= (totalPos - 1))
	{
		return -1;
	}
	children[spaceLoc] = children[spaceLoc+2];
	spaceLoc += 2;
	children[spaceLoc] = ' ';
	moves[nMoves++] = 'h';
	if(nMoves > maxMoves)
	{
		return -2;
	}
	return 0;
}

int
HopRight()
{
	if(spaceLoc <= 2)
	{
		return -1;
	}
	children[spaceLoc] = children[spaceLoc-2];
	spaceLoc -= 2;
	children[spaceLoc] = ' ';
	moves[nMoves++] = 'H';
	if(nMoves > maxMoves)
	{
		return -2;
	}
	return 0;
}

// spaceLoc - 2 better be a girl
// while spaceLoc >= altLeft + 2, hop girls right
int
HopGirls()
{
	int ret;

	while(spaceLoc >= (altLeft + 2))
	{
		if(children[spaceLoc - 2] != 'G')
		{
			printf("HopGirls: child at spaceLoc (%d) - 2 (%d) not girl\n",
				spaceLoc, spaceLoc - 2);
			return -2;
		}
		if((ret = HopRight()) != 0)
		{
			printf("HopGirls: HopRight at spaceLoc (%d) returned %d\n",
				spaceLoc, ret);
			return -3;
		}
	}
	altLeft = spaceLoc + 1;
	return 0;
}

// spaceLoc + 2  better be a boy
// while spaceLoc <= altRight - 2, hop boys left
int
HopBoys()
{
	int ret;

	while(spaceLoc <= (altRight - 2))
	{
		if(children[spaceLoc + 2] != 'B')
		{
			printf("HopBoys: child at spaceLoc (%d) + 2 (%d) not boy\n",
				spaceLoc, spaceLoc + 2);
			return -2;
		}
		if((ret = HopLeft()) != 0)
		{
			printf("HopBoys: HopLeft at spaceLoc (%d) returned %d\n",
				spaceLoc, ret);
			return -3;
		}
	}
	altRight = spaceLoc - 1;
	return 0;
}

char outbuf[80];

void
PrintSoln(int probNum)
{
	int rem, off, amt;

	printf("%d %d\n", probNum, nMoves);
	off = 0;
	rem = nMoves;
	while(rem > 0)
	{
		amt = rem;
		if(amt > CHARS_PER_LINE)
		{
			amt = CHARS_PER_LINE;
		}
		::memcpy(&(outbuf[0]), &(moves[off]), amt);
		outbuf[amt] = 0;
		printf("%s\n", &(outbuf[0]));
		off += amt;
		rem -= amt;
	}
	printf("\n");
}
char inbuf[256];

int
main(int argc, char **argv)
{
	int nProb, curProb, probNum, nGirls, nBoys, i, ret;
	char tChild;

	if(fgets(&(inbuf[0]), 255, stdin) == NULL)
	{
		fprintf(stderr, "Read failed on problem count\n");
		return -1;
	}
	if(sscanf(&(inbuf[0]), "%d", &nProb) != 1)
	{
		fprintf(stderr, "Scan failed on problem count\n");
		return -2;
	}
	for(probNum = 1; probNum <= nProb ; probNum++)
	{
		if(fgets(&(inbuf[0]), 255, stdin) == NULL)
		{
			fprintf(stderr, "Read failed on problem data %d\n", probNum);
			return -3;
		}
		if(sscanf(&(inbuf[0]), "%d %d %d", &curProb, &nGirls, &nBoys) != 3)
		{
			fprintf(stderr, "Scan failed on problem data %d\n", probNum);
			return -4;
		}
		if(curProb != probNum)
		{
			fprintf(stderr, "Scanned problem number %d != computed %d\n",
				curProb, probNum);
			return -5;
		}
		if((nGirls + nBoys) > MAX_CHILDREN)
		{
			fprintf(stderr, "nGirls (%d) + nBoys (%d) > max (%d) problem %d\n",
				nGirls, nBoys, MAX_CHILDREN, probNum);
			return -6;
		}
		if((ret = InitPos(nGirls,nBoys)) != 0)
		{
			printf("prob %d InitPos returned %d\n",
				probNum, ret);
			return -7;
		}
		if(nGirls == 0)
		{
			i = 0;
			while(i < nBoys)
			{
				if(i < (nBoys - 1))
				{
					if((ret = HopLeft()) != 0)
					{
						printf("No girls prob %d step %d hop returned %d\n",
							probNum, i+1, ret);
						return -8;
					}
					i += 2;
				}
				else
				{
					if((ret = SlideLeft()) != 0)
					{
						printf("No girls prob %d step %d slide returned %d\n",
							probNum, i+1, ret);
						return -9;
					}
					i++;
				}
			}
			PrintSoln(probNum);
		}
		else if(nBoys == 0)
		{
			i = 0;
			while(i < nGirls)
			{
				if(i < (nGirls - 1))
				{
					if((ret = HopRight()) != 0)
					{
						printf("No boys prob %d step %d hop returned %d\n",
							probNum, i+1, ret);
						return -10;
					}
					i += 2;
				}
				else
				{
					if((ret = SlideRight()) != 0)
					{
						printf("No boys prob %d step %d slide returned %d\n",
							probNum, i+1, ret);
						return -11;
					}
					i++;
				}
			}
			PrintSoln(probNum);
		}
		else
		{	// at least one boy and at least one girl
			// move left most boy next to
			// rtmost girl to form gb alternate len 2
		//	DumpConfig();
			if((ret = SlideLeft()) != 0)
			{
				printf("BG alt init prob %d slide returned %d\n",
					probNum, ret);
				return -12;
			}
			altRight = spaceLoc - 1;
			altLeft = spaceLoc - 2;
			nBadBoys--;
			nBadGirls--;
			while(altRight >= altLeft)
			{	// is space at right or left end of alt string
		//		DumpConfig();
				if(spaceLoc == (altRight + 1))
				{	// space to rt of rt end
					if(children[altRight] == 'B')
					{	// if rt is a boy, hop all alt girls right
						tChild = children[spaceLoc+1];	// remeber to decide where we are
						if(HopGirls() != 0)
						{
							return -13;
						}
						if(tChild == 'B')
						{	// if child to rt of prev empty, now girl is a boy
							// we have added another to the alt chain else another girsl in place
							altRight += 2;
						}
						else
						{
							leftGoodGirl--;
						}
					}
					else
					{	// if rt is a girl
						if(children[spaceLoc+1] == 'B')
						{	// childon other side is a boy slide him left
							if((ret = SlideLeft()) != 0)
							{
								printf("alt rt girl: nxt is boy, ret %d cannot slide spaceLoc %d\n",
									ret, spaceLoc);
								return -14;
							}
							altRight++;
						}
						else
						{	// if rt of space is a girl, move current girl over, now in place
							if((ret = SlideRight()) != 0)
							{
								printf("alt rt girl: nxt is girl, ret %d cannot slide spaceLoc %d\n",
									ret, spaceLoc);
								return -15;
							}
							altRight--;
							leftGoodGirl--;
						}
					}
				}
				else if(spaceLoc == (altLeft - 1))
				{
					if(children[altLeft] == 'G')
					{	// if left is a girl, hop all alt boys left
						tChild = children[spaceLoc-1];	// remeber to decide where we are
						if(HopBoys() != 0)
						{
							return -13;
						}
						if(tChild == 'G')
						{	// if child to left of prev empty, now boy is a girl
							// we have added another to the alt chain else another boy is in place
							altLeft -= 2;
						}
						else
						{
							rightGoodBoy++;
						}
					}
					else
					{	// if left is a boy
						if(children[spaceLoc-1] == 'G')
						{	// childon other side is a girl slide her right
							if((ret = SlideRight()) != 0)
							{
								printf("alt left boy: nxt is girl, ret %d cannot slide spaceLoc %d\n",
									ret, spaceLoc);
								return -14;
							}
							altLeft--;
						}
						else
						{	// if left of space is a boy, move current boy over, now in place
							if((ret = SlideLeft()) != 0)
							{
								printf("alt left boy: nxt is boy, ret %d cannot slide spaceLoc %d\n",
									ret, spaceLoc);
								return -15;
							}
							altLeft++;
							rightGoodBoy++;
						}
					}
				}
				else
				{
					printf("altLeft %d altRight %d spaceLoc %d\n",
						altLeft, altRight, spaceLoc);
					return -30;
				}
			}
			PrintSoln(probNum);
		}
	}
	return 0;
}
