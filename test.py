MAX = 4
LEN = 2

agent = []
job = []

def clamped_diff(a_list: list, j_list: list) -> int:
  return sum(max(a_ - j_, 0) for (a_, j_) in zip(a_list, j_list))


def func(filter_func = lambda j, a1, a1c, a1r, a2, a2c, a2r: True, results: set = set(), a1_list: list = [], a2_list: list = [], j_list: list = [], rem: int = LEN - 1):
  for a1 in range(MAX + 1):
    a1_list.append(a1)
    for a2 in range(MAX + 1):
      a2_list.append(a2)
      for j in range(MAX + 1):
        j_list.append(j)
        if (rem == 0):
          a1_cost = clamped_diff(j_list, a1_list)
          a1_reward = clamped_diff(a1_list, j_list)
          a2_cost = clamped_diff(j_list, a2_list)
          a2_reward = clamped_diff(a2_list, j_list)
          if (filter_func(j_list, a1_list, a1_cost, a1_reward, a2_list, a2_cost, a2_reward)):
            print(f"Job: {j_list} | Worker: {a1_list} Cost: {a1_cost} + Reward: {a1_reward} | Client: {a2_list} Cost: {a2_cost} + Reward: {a2_reward} | Profit Available: {a2_reward - a1_cost}")
            results.add((tuple(j_list), tuple(a1_list), a1_cost, a1_reward, tuple(a2_list), a2_cost, a2_reward))
        else:
          func(filter_func, results, a1_list, a2_list, j_list, rem - 1)
        j_list.pop()
      a2_list.pop()
    a1_list.pop()

results = set()
func(lambda j, a1, a1c, a1r, a2, a2c, a2r: a1r < a1c < a2r < a2c and a2r - a1c == 2, results)

print(results)