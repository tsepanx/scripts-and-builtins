import dataclasses
import typing
import random
# import numpy as np

from matplotlib import pyplot as plt

# values_cnt = 1000
# norm_values = [random.normalvariate(50, 100) for _ in range(values_cnt)]

START_TIME = 8 * 60
END_TIME = 20 * 60


@dataclasses.dataclass
class BusArrivalEntity:
    time: int
    passengers_count: int


BusesList = list[BusArrivalEntity]


def single_day(
        bus_time_f=lambda: random.choice([5, 10])
) -> BusesList:
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
            next_bus_interval = bus_time_f()

        minutes_passed += 1
        last_bus_went += 1

    return buses_entities


def draw_hist(values: typing.Sequence, xlabel: str = None):
    plt.hist(values, density=True, bins=25)
    plt.xlabel(xlabel)
    plt.show()


experiments_cnt = 10 ** 4
experiments_list: list[BusesList] = [single_day() for _ in range(experiments_cnt)]

# --- 1 ---

# buses_cnt_values = [len(exp) for exp in experiments_list]
# draw_hist(buses_cnt_values, 'buses count')

# --- 2 ---

# values_list: list[int] = list()
#
# for exp in experiments_list:
#     bus_choice_i = random.randint(0, len(exp) - 1)
#     bus_choice = exp[bus_choice_i]
#
#     value = bus_choice.passengers_count
#     values_list.append(value)
#
# draw_hist(values_list, 'passengers in equally likely bus')
#
# print('Mean: ', np.mean(values_list))
# print('Variance: ', np.var(values_list))


# --- 3 ---

# def nearest_bigger(a: BusesList, time: int) -> int:
#     i = 0
#     while i < len(a) - 1 and a[i].time <= time:
#         i += 1
#
#     return i
#
#
# values_list: list[int] = list()
#
# for exp in experiments_list:
#     timon_time_choice = random.randint(0, END_TIME - START_TIME)
#
#     if timon_time_choice > exp[-1].time:
#         values_list.append(0)
#         continue
#
#     next_bus_ind: int = nearest_bigger(exp, timon_time_choice)
#     next_bus = exp[next_bus_ind]
#
#     value = next_bus.passengers_count
#     values_list.append(value)
#
# draw_hist(values_list, 'passengers when arriving at equally likely time')
#
# print('Mean: ', np.mean(values_list))
# print('Variance: ', np.var(values_list))

# --- 4 ---

experiments_cnt = 10 ** 4
experiments_list: list[BusesList] = [single_day(
    bus_time_f=lambda x: random.expovariate(1 / 10)
) for _ in range(experiments_cnt)]
