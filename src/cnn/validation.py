def validate(num: int, indicator: bool, arr: list):
    if indicator:
        if ((num <= 94) or (2902 <= num <= 2942) or (5320 <= num <= 5393) or (7902 <= num <= 7908) or
                (8277 <= num <= 8327) or (num >= 11749)):
            arr[2] += 1
        else:
            arr[0] += 1
    else:
        if ((num <= 94) or (2902 <= num <= 2942) or (5320 <= num <= 5393) or (7902 <= num <= 7908) or
                (8277 <= num <= 8327) or (num >= 11749)):
            arr[1] += 1
        else:
            arr[3] += 1


def label(num: int) -> list:
    if ((num <= 94) or (2902 <= num <= 2942) or (5320 <= num <= 5393) or (7902 <= num <= 7908) or
            (8277 <= num <= 8327) or (num >= 11749)):
        return [[1, 0]]
    else:
        return [[0, 1]]
