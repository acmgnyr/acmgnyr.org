#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

typedef unsigned short WORD;
typedef unsigned char BYTE;
char inbuf[256];

/*
 * this is a standard recursive sudoku solver with two additions
 * 1. An initila scan of the sum constraints to see if they restrict the
 *		available values in each box (check_constraints)
 * 2. at each recursive search step, once we have set a value in a row & col
 *		and used sudoku constrints to remove available values in the row, col and 3x3 box
 *		scan through the sum constraints to see if they further restrict the
 *		available values in each box (check_constraints)
 */

int constraints[15][9];
WORD valid_masks[10] = {0, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x100};
#define ALL_MASK	0x1ff

/*
 * depth first search stack struct for sudoku
 */
typedef struct _search_state_
{
	WORD avail_mask[9][9];	// which values (valid_masks) can still be used in thsi box
	BYTE row_avail_counts[9][9];	// for each row, counts of how many boxes in the row have each value available
	BYTE col_avail_counts[9][9];	// for each col, counts of how many boxes in the col have each value available
	BYTE box_avail_counts[3][3][9];	// for each 3x3 box, counts of how many boxes in the 3x3 box have each value available
	BYTE val_set[9][9];	// if non-zero, thsi box is set t the current value in the current search
} SEARCH_STATE;

SEARCH_STATE states[81];

/*
 * intialize the search stack to no values chosen and all values available
 * in all boxes
 */
void search_init()
{
	SEARCH_STATE *pss = &(states[0]);
	int i, j, k;
	for(i = 0; i < 9; i++) {
		for(j = 0; j < 9 ; j++) {
			pss->avail_mask[i][j] = ALL_MASK;
			pss->val_set[i][j] = 0;
			pss->row_avail_counts[i][j] = 9;
			pss->col_avail_counts[i][j] = 9;
		}
	}
	for(i = 0; i < 3; i++) {
		for(j = 0; j < 3 ; j++) {
			for(k = 0; k < 9 ; k++){
				pss->box_avail_counts[i][j][k] = 9;
			}
		}
	}
}

/*
 * convert string of <=> to array of -1 0 1
 */
int scan_convert(int *prow, int n, const char *s)
{
	int i;

	for(i = 0; i < n; i++, s++){
		switch(*s){
		case '<': *prow++ = -1; break;
		case '=': *prow++ = 0; break;
		case '>': *prow++ = 1; break;
		default: return(i);
		}
	}
	return(i);
}

// read constraints from stdin
int scan_constraints()
{
	int i, j, n;
	for(i = 0; i < 3; i++) {
		for(j = 0; j < 3; j++) {
			if(fgets(&(inbuf[0]), 255, stdin) == NULL)
			{
				fprintf(stderr, "Read of first line of region %d block %d failed\n", j, i);
				return -21;
			}
			n = scan_convert(&(constraints[5*i+2*j][0]), 6, &(inbuf[0]));
			if(n != 6){
				fprintf(stderr, "Scan of first line of region %d block %d failed got %d wanted 6: %s", j, i, n, &(inbuf[0]));
				return(-22);
			}
			if(j < 2) {
				if(fgets(&(inbuf[0]), 255, stdin) == NULL)
				{
					fprintf(stderr, "Read of second line of region %d block %d failed\n", j, i);
					return -23;
				}
				n = scan_convert(&(constraints[5*i+2*j+1][0]), 9, &(inbuf[0]));
				if(n != 9) {
					fprintf(stderr, "Scan of second line of region %d block %d failed got %d wanted 9: %s\n", j, i, n, &(inbuf[0]));
					return -24;
				}
			}
		}
	}
	return 0;
}

/*
 * see if any of the avail values in baseMask can be eliminated because value in chkMask must
 * sum to 10 with baseMask value
 * return mask of values to be removed
 */
WORD checkEqual(WORD baseMask, WORD chkMask)
{
	WORD result = 0;
	int i;
	// cannot be a 5
	if(valid_masks[5] & baseMask) {
		result |= valid_masks[5];
	}
	// if any value not available in chkMask, 10 - that not available in base
	for(i = 1; i <= 9 ; i++) {
		if(((valid_masks[i] & chkMask) == 0) && (valid_masks[10-i] & baseMask)) {
			result |= valid_masks[10-i];
		}
	}
	return result;
}

/*
 * see if any of the avail values in baseMask can be eliminated because value in chkMask must
 * sum to less than 10 with baseMask value
 * return mask of values to be removed
 */
WORD checkLess(WORD baseMask, WORD chkMask)
{
	WORD result = 0;
	int i;
	// cannot be a 9
	if(valid_masks[9] & baseMask) {
		result |= valid_masks[9];
	}
	// if 1 not avail in chkMask, cannot use 8 etc
	for(i = 1; i <= 8 ; i++) {
		if((valid_masks[i] & chkMask) != 0) {
			break;
		} else if(valid_masks[9-i] & baseMask) {
			result |= valid_masks[9-i];
		}
	}
	return result;
}

/*
 * see if any of the avail values in baseMask can be eliminated because value in chkMask must
 * sum to greater than 10 with baseMask value
 * return mask of values to be removed
 */
WORD checkGreater(WORD baseMask, WORD chkMask)
{
	WORD result = 0;
	int i;
	// cannot be a 1
	if(valid_masks[1] & baseMask) {
		result |= valid_masks[1];
	}
	// if 9 not avail in chkMask, cannot use 2 etc
	for(i = 9; i >= 3 ; i--) {
		if((valid_masks[i] & chkMask) != 0) {
			break;
		} else if(valid_masks[11-i] & baseMask) {
			result |= valid_masks[11-i];
		}
	}
	return result;
}

/*
 * determine if any values available in the current box (baseMask) can
 * be excluded by applying constraint with the available values in the neighboring box (chkMask)
 * return mask of values to be REMOVED
 */
WORD checkConstraint(int constraint, WORD baseMask, WORD chkMask)
{
	if(constraint < 0) {
		return checkLess(baseMask, chkMask);
	} else if(constraint > 0) {
		return checkGreater(baseMask, chkMask);
	} else {
		return checkEqual(baseMask, chkMask);
	}
}

/*
 * for each box, check if constraints with respect to neighboring boxes eliminate
 * any available values
 * return -1 if some box has no remaining available values
 */
int check_constraints(SEARCH_STATE *pss)
{
	int i, row, col, baseConsRow, baseConsCol, scan_count, change_count = 1;
	WORD baseMask, chkMask, resultMask, totResult;
	scan_count = 0;
	// sacn all constraints until on more changes
	while(change_count > 0) {
		scan_count++;
		change_count = 0;
		for(row = 0, baseConsRow = 0; row < 9 ; row++) {
			for(col = 0, baseConsCol = 0; col < 9 ; col++) {
				if(pss->val_set[row][col] == 0) {	// if we have not already set thsi box in search see if it can change
					baseMask = pss->avail_mask[row][col];
					totResult = 0;	// all changes to baseMask
					if((col %3) != 0) {	// if not first col in 3x3 block, check constraint with box to left
						chkMask = pss->avail_mask[row][col-1];
						resultMask = checkConstraint(constraints[baseConsRow][baseConsCol-1], baseMask, chkMask);
						if(resultMask != 0) {
							baseMask &= ~resultMask;
							change_count++;
							totResult |= resultMask;
						}
					}
					if((col %3) != 2) {	// if not last col in 3x3 block, check constraint with box to right
						chkMask = pss->avail_mask[row][col+1];
						resultMask = checkConstraint(constraints[baseConsRow][baseConsCol], baseMask, chkMask);
						if(resultMask != 0) {
							baseMask &= ~resultMask;
							change_count++;
							totResult |= resultMask;
						}
					}
					if((row %3) != 0) {	// if not top row in 3x3 block, check constraint with box above
						chkMask = pss->avail_mask[row-1][col];
						resultMask = checkConstraint(constraints[baseConsRow-1][col], baseMask, chkMask);
						if(resultMask != 0) {
							baseMask &= ~resultMask;
							change_count++;
							totResult |= resultMask;
						}
					}
					if((row %3) != 2) {	// if not bottom row in 3x3 block, check constraint with box below
						chkMask = pss->avail_mask[row+1][col];
						resultMask = checkConstraint(constraints[baseConsRow+1][col], baseMask, chkMask);
						if(resultMask != 0) {
							baseMask &= ~resultMask;
							change_count++;
							totResult |= resultMask;
						}
					}
					if(baseMask == 0) {	// not already set and no values available for box
						return -1;
					}
					pss->avail_mask[row][col] = baseMask; 
					if(totResult != 0) {	// adjust counts for boxes in same row, col or 3x3 box
						for(i = 0; i < 9 ; i++) {
							if(valid_masks[i] & totResult) {
								pss->col_avail_counts[col][i-1]--;
								pss->row_avail_counts[row][i-1]--;
								pss->box_avail_counts[row/3][col/3][i-1]--;
							}
						}
					}
				}
				if((col %3) != 2) {	// advance to next row constraint if not last col in 3x3 box
					baseConsCol++;
				}
			}
			if((row % 3) != 2) {	// if not last row of 3x3 box used 2 rows of constrsints, else 1
				baseConsRow += 2;
			} else {
				baseConsRow++;
			}
		}
	}
	return 0;
}

/*
 * depth first search step choice info
 */
#define STYP_ROW	1	// depth first search on boxes with a value available in a given row
#define STYP_COL	2	// depth first search on boxes with a value available in a given col
#define STYP_BOX	3	// depth first search on boxes with a value available in a given 3x3 box
typedef struct _solve_data_ {
	int solve_type;
	int solve_val;
	int solve_row;
	int solve_col;
	int solve_cnt;
	int solve_index;
	int test_row;
	int test_col;
} SOLVE_DATA;

/*
 * choose how to branch for the next sudoku depth first search step
 * choose the branching with the smallest number of children in the tree
 * either the sammlest number of values still available in a box (row & col)
 * or the smallest number of boxes in a row, col or 3x3 sub-box with a particular value available
 * on return *psd has the information on the branch choice
 */
int GetSolveStep(SEARCH_STATE *pss, SOLVE_DATA *psd)
{
	int i, j, k;
	psd->solve_cnt = 10;
	for(i = 0; i < 9; i++) {
		for(j = 0; j < 9; j++) {
			if(pss->row_avail_counts[i][j] < psd->solve_cnt) {
				psd->solve_cnt = pss->row_avail_counts[i][j];
				psd->solve_type = STYP_ROW;
				psd->solve_row = i;
				psd->solve_val = j+1;
			}
		}
	}
	for(i = 0; i < 9; i++) {
		for(j = 0; j < 9; j++) {
			if(pss->col_avail_counts[i][j] < psd->solve_cnt) {
				psd->solve_cnt = pss->col_avail_counts[i][j];
				psd->solve_type = STYP_COL;
				psd->solve_col = i;
				psd->solve_val = j+1;
			}
		}
	}
	for(i = 0; i < 3; i++) {
		for(j = 0; j < 3; j++) {
			for(k = 0; k < 9 ; k++) {
				if(pss->box_avail_counts[i][j][k] < psd->solve_cnt) {
					psd->solve_cnt = pss->box_avail_counts[i][j][k];
					psd->solve_type = STYP_BOX;
					psd->solve_row = i;
					psd->solve_col = j;
					psd->solve_val = k+1;
				}
			}
		}
	}
	if(psd->solve_cnt == 0) {	// if some value has no choices left, fail
		return -1;
	} else {
		return 0;
	}
}

/*
 * after we have made a branch choice, we have to try each of the branches
 * at the search node bay scanning through boxes in the selected row, col or 3x3 box
 * or values within a selected box, thsi routine steps to the next choice
 * for first try, start with 0, else start with location after previous try
 * and scan to the next available box or value
 */
int FindNextTest(SEARCH_STATE *pss, SOLVE_DATA *psd)
{
	int i, j, starti, startj;
	WORD mask = valid_masks[psd->solve_val];
	if(psd->solve_index >= psd->solve_cnt) {
		return -1;
	}
	switch(psd->solve_type) {
	case STYP_ROW:
		if(psd->solve_index == 0) {
			startj = 0;
		} else {
			startj = psd->test_col+1;
		}
		i = psd->solve_row;
		for(j = startj ; j < 9 ; j++) {
			if(pss->avail_mask[i][j] & mask) {
				psd->test_col = j;
				psd->test_row = i;
				psd->solve_index++;
				return 0;
			}
		}
		return -1;
	case STYP_COL:
		if(psd->solve_index == 0) {
			starti = 0;
		} else {
			starti = psd->test_row+1;
		}
		j = psd->solve_col;
		for(i = starti ; i < 9 ; i++) {
			if(pss->avail_mask[i][j] & mask) {
				psd->test_col = j;
				psd->test_row = i;
				psd->solve_index++;
				return 0;
			}
		}
		return -1;
	case STYP_BOX:
		if(psd->solve_index == 0) {
			starti = 0;
			startj = 0;
		} else {
			starti = psd->test_row - 3*psd->solve_row;
			startj = psd->test_col+1 - 3*psd->solve_col;
		}
		for(i = starti; i < 3 ; i++) {
			for(j = startj ; j < 3; j++) {
				if(pss->avail_mask[i + 3*psd->solve_row][j + 3*psd->solve_col] & mask) {
					psd->test_col = j + 3*psd->solve_col;
					psd->test_row = i + 3*psd->solve_row;
					psd->solve_index++;
					return 0;
				}
			}
		}
		return -1;
	default:
		fprintf(stderr, "bad solve type %d\n", psd->solve_type);
		return -1;
	}
}

/*
 * apply a choice to set a box at row, col,to value
 * then clear all other avail masks at row, col and
 * apply sudoku constraints to decrement counts
 * for other boxes in row, col and 3x3 box not already set,
 * remove val from avail_masks and decrement counts
 */
int ApplyChoice(SEARCH_STATE *pss, int row, int col, int val)
{
	int i, j, boxr, boxc;
	WORD mask = valid_masks[val];
	if(pss->val_set[row][col] != 0) {
		fprintf(stderr, "ApplyChoice: row %d col %d val %d already set to %d\n", row, col, val, pss->val_set[row][col]);
		return -1;
	}
	pss->val_set[row][col] = val;
	// remove val from other avails in row, col and box
	boxr = row/3;
	boxc = col/3;
	for(j = 0; j < 9 ; j++) {
		if(pss->avail_mask[row][j] & mask) {	// reduce counts
			pss->box_avail_counts[boxr][j/3][val-1]--;
			pss->col_avail_counts[j][val-1]--;
		}
		pss->avail_mask[row][j] &= ~mask;
	}
	for(i = 0; i < 9 ; i++) {
		if(pss->avail_mask[i][col] & mask) {
			pss->box_avail_counts[i/3][boxc][val-1]--;
			pss->row_avail_counts[i][val-1]--;
		}
		pss->avail_mask[i][col] &= ~mask;
	}
	boxr = row/3;
	boxc = col/3;
	for(i = 3*boxr; i < 3*(boxr+1); i++) {
		for(j = 3*boxc; j < 3*(boxc+1); j++) {
			if(pss->avail_mask[i][j] & mask) {
				pss->col_avail_counts[j][val-1]--;
				pss->row_avail_counts[i][val-1]--;
			}
			pss->avail_mask[i][j] &= ~mask;
		}
	}
	//for each other value at row/col, decrement ist counts
	for(i = 1; i <= 9 ; i++) {
		if((i != val) && ((pss->avail_mask[row][col] & valid_masks[i]) != 0)){
			pss->box_avail_counts[row/3][col/3][i-1]--;
			pss->col_avail_counts[col][i-1]--;
			pss->row_avail_counts[row][i-1]--;
		}
	}
	pss->avail_mask[row][col] = mask;	// no one can use it but we need to tell neighbors that other vals not avail
	pss->row_avail_counts[row][val-1] = 32;	// never choose the row and val again
	pss->col_avail_counts[col][val-1] = 32;	// never choose the col and val again
	pss->box_avail_counts[boxr][boxc][val-1]= 32;	// never choose the box and val again
	return 0;
}

/*
 * sudoku recursive step
 * at given level, copy current state to next slot, 
 * pick a branch row col or 3x3 box and value
 * if we are down the last choice (depth = 80) just set the (only remaining) choice and return 0
 * for each branch. set the value at the row & col to the appropriate value
 * and update avail counts to reflect sudoku constraints
 * then chack sum constraints
 * if any box has no more available values, go on to the next choice on the branch
 * else call Solve recursively
 * If the recursive call returns -1, go on to the next choice on the branch
 *  and if you run out of boxes return -1
 * if recursive call returns 0, copy soln (val_set from next slot to current and return 0
 * so when top level returns, val_set is the soln
 */
int Solve(int level)
{
	SEARCH_STATE *pssnxt, *pss = &(states[level]);
	SOLVE_DATA sd;
	int i, j;
	if(GetSolveStep(pss, &sd) != 0) {	// find row, col or 3x3 box) + value to scan
		return -1;
	}
#ifdef DEBUG
	printf("level %d solve type %d row %d col %d val %d cnt %d\n", level,
		sd.solve_type, sd.solve_row, sd.solve_col, sd.solve_val, sd.solve_cnt);
#endif
	sd.solve_index = 0;
	while(FindNextTest(pss, &sd) == 0) {	// for each candidate in chosen row, col or 3x3 box, get row& col to set
		if(level == 80) {	// if this is the last box to fill in we are done, set it and return 0 (success)
			pss->val_set[sd.test_row][sd.test_col] = sd.solve_val;
			return 0;
		} else {	// else copy current to next and try each possiblility
			pssnxt = &(states[level+1]);
			*pssnxt = *pss;
#ifdef DEBUG
			fprintf(stderr, "try row %d col %d val %d\n", sd.test_row, sd.test_col, sd.solve_val);
#endif
			if(ApplyChoice(pssnxt, sd.test_row, sd.test_col, sd.solve_val) == 0) { // set this value choice
#ifdef DEBUG
				check_soln(pssnxt);
#endif
				if(check_constraints(pssnxt) == 0) {	// if not killed by constraints	
#ifdef DEBUG
					check_soln(pssnxt);
#endif
					if(Solve(level+1) == 0) {	// call solve recursively, if success, copy val_set
						for(i = 0; i < 9; i++) {
							for(j = 0; j < 9 ; j++) {
								pss->val_set[i][j] = pssnxt->val_set[i][j];
							}
						}
						return 0;
					}
				}
#ifdef DEBUG
			} else {
				fprintf(stderr, "Apply Choice failed at level %d \n", level);
#endif
			}
		}
	}
	return -1;
}

int main()
{
	int nprob, curprob, index, ret, i, j;
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
		// get prob num degree
		if(sscanf(&(inbuf[0]), "%d", &index) != 1)
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
		search_init();	// init first atate
		if((ret = scan_constraints()) != 0) {	// read constriants
			return ret;
		}
		if(check_constraints(&(states[0])) != 0) { // apply constraints to first state
			fprintf(stderr, "problem index %d init check constraints failed\n",
				index);
			return -8;
		}
#ifdef DEBUG
		if(check_soln(&(states[0])) != 0) {
			fprintf(stderr, "problem index %d after init check constraints no match\n",
				index);
			return -18;
		}
#endif
		if(Solve(0) != 0) {	// call solve
			fprintf(stderr, "problem index %d no solution\n",
				index);
			return -9;
		}
		// if it works, print out soln
		printf("%d\n", index);
		for(i = 0; i < 9 ; i++) {
			for(j = 0; j < 9 ; j++) {
				printf("%d ", states[0].val_set[i][j]);
			}
			printf("\n");
		}
	}
	return 0;
}

