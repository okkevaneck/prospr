import random


if __name__ == "__main__":
    ids_to_replace = [1230]
    aminos = ["H", "P"]
    h_ratio_low, h_ratio_high = 0.2, 0.3
    p_len = 30
    dataset = "vanEck_hratio"
    filename = f"{dataset}/{''.join(aminos)}{p_len}_r{h_ratio_high}.csv"
    new_size = 1000

    with open(filename, "r") as csvfile:
        data = csvfile.readlines()

    proteins = set([d.strip().split(",")[1] for d in data])

    # Create new proteins.
    new_proteins = set(
        [
            "".join(
                random.choices(
                    aminos,
                    weights=[
                        min(h_ratio_high, 0.9),
                        max(round(1 - h_ratio_high, 1), 0.1),
                    ],
                    k=p_len,
                )
            )
            for _ in range(new_size)
        ]
    )

    # Filter newly generated proteins on the H-ratio requirements.
    new_proteins = set(
        [
            p
            for p in new_proteins
            if p.count("P") != 0
            and h_ratio_low < p.count("H") / p_len <= h_ratio_high
        ]
    )
    new_set = new_proteins - proteins

    # Replace the provided ids with new proteins.
    for repl_id in ids_to_replace:
        data[repl_id + 1] = f"{repl_id},{new_set.pop()}\n"

    # Overwrite all proteins with new set.
    with open(filename, "w") as csvfile:
        csvfile.writelines(data)
