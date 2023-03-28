import dataclasses
import typing

from matplotlib import pyplot as plt
import random

# X_START = 0
# X_END = 1000
# Y_TOP = 500
# Y_BOTTOM = -Y_TOP

values_cnt = 1000
norm_values = [random.normalvariate(50, 100) for _ in range(values_cnt)]

START_TIME = 8 * 60
END_TIME = 20 * 60


@dataclasses.dataclass
class BusArrivalEntity:
    time: int
    passengers_count: int


BusesList = list[BusArrivalEntity]


class DayData:
    buses_times_list: BusesList


def single_day() -> BusesList:
    buses_entities: BusesList = list()

    minutes_passed = 0
    last_bus_went = 0
    next_bus_interval = 0
    while minutes_passed < (END_TIME - START_TIME):
        if last_bus_went == next_bus_interval:
            # <last_bus_went> passengers went away on the bus
            b_entity = BusArrivalEntity(
                time=minutes_passed,
                passengers_count=last_bus_went
            )
            buses_entities.append(b_entity)

            last_bus_went = 0
            next_bus_interval = random.choice([5, 10])

        minutes_passed += 1
        last_bus_went += 1

    return buses_entities


def draw_evenly(
        values: typing.Sequence,
        title: str = None,
        x_start=0,
        # x_end=X_END,
        # y_bottom=Y_BOTTOM,
        # y_top=Y_TOP,
        is_sorted=True,
        invert_axis=True
):
    y_coords = values

    x_end = len(values) + x_start
    x_coords = [i for i in range(x_start, x_end)]

    y_bottom = min(values)
    y_top = max(values)

    if is_sorted:
        y_coords = sorted(y_coords)

    if invert_axis:
        x_start, x_end, y_bottom, y_top = y_bottom, y_top, x_start, x_end
        x_coords, y_coords = y_coords, x_coords

    plt.grid()
    plt.rcParams["figure.autolayout"] = True
    if title:
        plt.suptitle(title)
    plt.xlim(x_start, x_end)
    plt.ylim(y_bottom, y_top)

    plt.hist(x_coords)
    # plt.plot(x_coords, y_coords)
    plt.show()

# draw_evenly(
#     norm_values,
#     # 0,
#     # -500, 500,
#     is_sorted=True,
#     invert_axis=True
# )

experiments_cnt = 10 ** 4

# --- 1 ---

buses_cnt_values = [len(single_day()) for _ in range(experiments_cnt)]
draw_evenly(buses_cnt_values, 'buses count (by x)')

# --- 2 ---
# values_list: list[int] = list()
#
# for i in range(experiments_cnt):
#     buses_list: BusesList = single_day()
#
#     bus_choice_i = random.randint(0, len(buses_list) - 1)
#     bus_choice = buses_list[bus_choice_i]
#
#     values_list.append(bus_choice.passengers_count)
#
# draw_evenly(values_list, 'passengers count in random bus', invert_axis=False)