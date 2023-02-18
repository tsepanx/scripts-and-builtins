from typing import Iterable, Sequence


class Course:
    name: str
    cur_sum = 0
    cur_max = 0
    cur_sum_real = 0

    # min_c_points = 60

    def __init__(
            self,
            name: str,
            min_c_points: float,
            grades: Iterable[Sequence[float]]):
        self.name = name
        self.min_c_points = min_c_points
        for i in grades:
            a = i[0]
            b = i[1]
            c = i[2] if len(i) > 2 else 1
            self.add_new_grade(a, b, c)

    @property
    def left_max(self):
        return 100 - self.cur_max

    @property
    def left_needed(self):
        return self.min_c_points - self.cur_sum

    @property
    def real_s(self):
        return f"{self.cur_sum}" if self.cur_sum else f"({self.cur_sum_real})"

    def cur_ratio(self):
        return self.cur_sum / self.cur_max

    def add_new_grade(self, grade: float, max_grade: float, weight_coeff: int = 1):
        self.cur_sum += grade * weight_coeff
        self.cur_max += max_grade * weight_coeff

        # self.cur_max = round(self.cur_max, 2)
        # self.cur_sum = round(self.cur_sum, 2)

        if max_grade != 0:
            self.cur_sum_real = self.cur_sum

    def needed_ratio_for_left(self):
        if self.left_max == 0:
            if self.left_needed == 0:
                return None
            elif self.left_needed > 0:
                return "YOU GO TO RETAKE!"
            elif self.left_needed < 0:
                return "Completed !!!"
        res = self.left_needed / self.left_max
        res = round(res, 2)
        if res > 1:
            return f"You WILL go to Retake!: {res}"
        return res

    def __str__(self):
        return f"CUR: {self.real_s} / {self.cur_max} | " \
               f"LEFT NEEDED: {round(self.left_needed, 2)} / {self.left_max} [{self.needed_ratio_for_left()}]"

    def str_format(self, by_str: str):
        print(f"\n--- {self.name} ---")
        return by_str.format(f'{self.real_s} / {self.cur_max}', f'{round(self.left_needed, 2)} / {self.left_max}',
                             f'[{self.needed_ratio_for_left()}]')


offsets = [20, 20, 15]
offsets_s = ' '.join([f"{{:<{i}}}" for i in offsets])

print(offsets_s.format('CUR', 'LEFT NEEDED', 'Percent LEFT'))

courses = []


c = Course('Diffurs', 55, [
    (1, 15),  # Shilov theor 1
    (7, 15),  # Practical 1
    (12, 0),  # Bonus points
    # (3, 0),   # Collocvium bonus points
    (15, 15),  # Computational test

    (11.5, 15),  # --- 2nd practical test
    (4, 15),  # --- (THeor 2) Shilov test
    # (4, 25)   # --- Final theor Oral
])
# courses.append(c)
print(c.str_format(offsets_s))

c = Course('OS\'i', 60, [
    [8.3, 10],  # Quizzes
    [30 * 0.7833, 30],  # HW's (Labs)
    [10.5, 20],  # Midterm
    # [15, 20],  # Final
    # [16, 20],  # Unattended Oral

    [5, 0],  # Participation
])
# courses.append(c)
# print(c.str_format(offsets_s))

c = Course('Optimization', 60, [
    (7.5, 15),  # Test 1
    (17, 30),  # Midterm
    (11, 15),  # Test 2
    # (25, 40),  # Final
])
# print(c.str_format(offsets_s))
# courses.append(c)

c = Course('Probstat', 50, [
    [12, 14],  # Test1
    [12, 20],  # Midterm
    [7, 14],  # Test2
    # [0, 50], # Final
])
print(c.str_format(offsets_s))
# courses.append(c)

c = Course('Intro to AI (Philo)', 60, [
    [25 * 0.65, 25],  # Midterm
    [20 * 0.82, 20],  # HW1
    [9, 10],  # Attendance
    # [1, 0], # Bonus
    [20 * 0.89, 20], #HW2
    [0.57 * 25, 25],  # Final
])
# courses.append(c)


c = Course('Physics', 50, [
    # (3, 5),     # Attendance
    (6.4, 10),  # Quiz 1
    (5.3, 10),  # Quiz 2
    (7.5, 10),  # Quiz 3
    (5, 10),  # Quiz 4
    (0, 10),  # Quiz 5
    (3.75, 5),  # Assignment 1
    (3.75, 5),  # Assignment 2
    (5, 5),  # Assignment 3
    (3.75, 5),  # Assignment 4
    (4, 5),  # Assignment 5
    # (0, 25),    # Final
])
print(c.str_format(offsets_s))
# courses.append(c)

def sort_by(x):
    a = x.needed_ratio_for_left()
    if isinstance(a, float):
        # print(a)
        return a
    return 0


courses.sort(key=sort_by, reverse=True)
c: Course
# for c in courses:
#     print(c.str_format(offsets_s))
