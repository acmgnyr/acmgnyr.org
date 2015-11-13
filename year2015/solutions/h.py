# H - Brocard Point
# ICPC GNYR 2015
# Solution by: Sean McIntyre
# Problem by: Fred Pickel
#
import sys
import math
from math import sqrt, cos, sin, atan2

EPS = 1e-9

def line_intersection(line1, line2):
    xdiff = (line1[0][0] - line1[1][0], line2[0][0] - line2[1][0])
    ydiff = (line1[0][1] - line1[1][1], line2[0][1] - line2[1][1]) #Typo was here

    def det(a, b):
        return a[0] * b[1] - a[1] * b[0]

    div = det(xdiff, ydiff)
    if div == 0:
       raise Exception('lines do not intersect')

    d = (det(*line1), det(*line2))
    x = det(d, xdiff) / div
    y = det(d, ydiff) / div
    return x, y


def angle_between_lines(line1, line2):
	angle1 = atan2(line1[0][1] - line1[1][1], line1[0][0] - line1[1][0])
	angle2 = atan2(line2[0][1] - line2[1][1], line2[0][0] - line2[1][0])
	res = normalize(angle1 - angle2)

	return res

def go(ax, ay, bx, by, cx, cy):
	min_angle = 0.0
	max_angle = math.pi / 6 + EPS

	line_ab = ((ax, ay), (bx, by))
	line_bc = ((bx, by), (cx, cy))
	line_ca = ((cx, cy), (ax, ay))
	line_unit = ((0, 0), (1, 0))

	ab_angle = angle_between_lines(line_ab, line_unit)
	bc_angle = angle_between_lines(line_bc, line_unit)
	ca_angle = angle_between_lines(line_ca, line_unit)

	while max_angle - min_angle > EPS:
		# print 'min %.3f max %.3f' % (min_angle, max_angle)

		angle = (min_angle + max_angle) / 2.0

		ab_prime_angle = ab_angle + angle
		bc_prime_angle = bc_angle + angle

		ab_prime_line = ((ax, ay), (ax + cos(ab_prime_angle), ay + sin(ab_prime_angle)))
		bc_prime_line = ((bx, by), (bx + cos(bc_prime_angle), by + sin(bc_prime_angle)))

		brocade = line_intersection(ab_prime_line, bc_prime_line)

		ca_prime_line = ((cx, cy), brocade)
		ca_prime_angle = angle_between_lines(ca_prime_line, line_ca)

		# print ' --> angle %.1f ca_prime_angle %.1f' % (angle / math.pi * 180, ca_prime_angle / (math.pi) * 180)
		# print ' --> brocade (%.2f, %.2f)' % (brocade[0], brocade[1])

		if ca_prime_angle > angle:
			min_angle = angle
		else:
			max_angle = angle

	return brocade


def to_degrees(r):
	return r / math.pi * 180


def normalize(r):
	while r < -math.pi:
		r += 2*math.pi

	while r >= math.pi:
		r -= 2*math.pi

	return r


def check(ax, ay, bx, by, cx, cy, brocade):
	line_ab = ((ax, ay), (bx, by))
	line_bc = ((bx, by), (cx, cy))
	line_ca = ((cx, cy), (ax, ay))
	line_pa = (brocade, (ax, ay))
	line_pb = (brocade, (bx, by))
	line_pc = (brocade, (cx, cy))

	pab = normalize(angle_between_lines(line_pa, line_ab))
	pbc = normalize(angle_between_lines(line_pb, line_bc))
	pca = normalize(angle_between_lines(line_pc, line_ca))

	# print "pab %.2f pbc %.5f pca %.5f" % (to_degrees(pab), to_degrees(pbc), to_degrees(pca))

	assert abs(pab - pbc) < 1e-6
	assert abs(pbc - pca) < 1e-6


if __name__ == '__main__':
	n_test_cases = int(sys.stdin.readline().strip())

	for c_test_case, line in enumerate(sys.stdin, start=1):
		test_case, ax, ay, bx, by, cx, cy = [float(x) for x in line.split(' ')]
		assert test_case == c_test_case

		brocade = go(ax, ay, bx, by, cx, cy)

		print '%d %.5f %.5f' % (test_case, brocade[0], brocade[1])

		check(ax, ay, bx, by, cx, cy, brocade)

	assert n_test_cases == c_test_case
