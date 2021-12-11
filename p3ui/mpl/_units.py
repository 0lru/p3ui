dpi = 92


def points_to_pixels(points):
    return points * dpi / 72


def pixels_to_points(pixels):
    return pixels * 72 / dpi
