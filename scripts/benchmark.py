"""Module for running google benchmarks."""

import google_benchmark as benchmark
import numpy as np

import dispy


def gen_rng_sets(gen: np.random.Generator, n_sets: int, set_size: int) -> np.ndarray:
    """Generates N binary vectors representing item sets.

    Args:
        gen: A numpy.random.Generator object.
        n_sets: A number of sets to generate.
        set_size: A set size.

    Returns:
        A 2d matrix as a numpy.ndarray object where each row is an unique set.
    """
    return gen.integers(2, size=[n_sets, set_size])


def gen_uniq_rng_set(
    n_sets: int, set_size: int, seed: int = 42, iterations: int = int(1e5)
) -> np.ndarray:
    """Generates N unique binary sets of fixed size.
    Args:
        n_sets: A number of sets to generate.
        set_size: A set size.
        seed: A seed used for constructing a random number generator.
        iterations: A number of iterations after wich the function quits
                    attempting to construct new unique subsets and returns
                    the best possible result.

    Returns:
        A 2d matrix as a numpy.ndarray object where each row is an unique set.
    """
    dst = np.empty((0, set_size), dtype=np.int32)
    gen = np.random.default_rng(seed=seed)
    for _ in range(int(iterations)):
        dst = np.unique(
            np.vstack(
                (
                    dst,
                    gen_rng_sets(
                        gen,
                        n_sets=n_sets - len(dst),
                        set_size=set_size,
                    ),
                )
            ),
            axis=0,
        )

        if len(dst) == n_sets:
            break

    return dst


def gen_uniform_indices(
    min_index,
    max_index,
    sample_size: int,
    seed=42,
) -> np.ndarray:
    """Generate random indices in a given range following an uniform distribution.

    Args:
        min_index: A min index in a population.
        max_index: A max index in a population.
        sample_size: A number of random indices to generate.
        seed: A random seed.

    Return:
        An array of uniformly sampled random indices.
    """
    np.random.seed(seed)
    return np.random.randint(min_index, max_index, sample_size)


def gen_noraml_indices(
    min_index: int,
    max_index: int,
    sample_size: int,
    seed=42,
) -> np.ndarray:
    """Generate random indices in a given range following the normal distribution.

    Args:
        min_index: A min index in a population.
        max_index: A max index in a population.
        sample_size: A number of random indices to generate.
        seed: A random seed.

    Return:
        An array random indices.
    """

    np.random.seed(seed)
    population_size = max_index - min_index

    return np.clip(
        np.random.normal(
            population_size / 2,
            np.sqrt(
                np.sum((np.arange(0, population_size) - (population_size / 2)) ** 2)
                / population_size
            )
            / 3,
            sample_size,
        ).astype(np.uint64),
        0,
        population_size,
    )


def sample_sets_uniform(sets: np.ndarray, size: int) -> np.ndarray:
    """Creates a uniform sample of fixed size from the array of sets
    encoded as binary vectors.

    Args:
        sets: A 2D matrix where each row is a set represented as a binary vector.
            sample_size.
        size: A sample size.

    Returns:
        A 2D matrix representing set sample.
    """

    return sets[gen_uniform_indices(0, len(sets), size)]


def sample_sets_normal(sets: np.ndarray, size: int) -> np.ndarray:
    """Creates a normal sample of fixed size from the array of sets
    encoded as binary vectors.

    Args:
        sets: A 2D matrix where each row is a set represented as a binary vector.
            sample_size.
        size: A sample size.

    Returns:
        A 2D matrix representing set sample.
    """

    return sets[gen_noraml_indices(0, len(sets), size)]


SAMPLING_FUNCTIONS = {
    "uniform": sample_sets_uniform,
    "normal": sample_sets_normal,
}


TB_ALGOS = {
    "copy": dispy.copy,
    "unique": dispy.unique,
    "unique_binary": dispy.unique_binary,
    "unique_ankerl": dispy.unique_ankerl,
}

SEARCH_SPACE = {
    "unique-subsets": [128, 256, 512],
    "subset-size": [64, 512, 2048],
    "sample-size": [64, 128, 512, 1024, 2048, 4096, 8192],
}


@benchmark.register
@benchmark.option.range_multiplier(2)
@benchmark.option.args_product(
    [
        [0, 1],  # sampling method
        [0, 1, 2, 3],  # algorithm index
        SEARCH_SPACE["subset-size"],
        SEARCH_SPACE["unique-subsets"],
        SEARCH_SPACE["sample-size"],
    ]
)
def tb_unique_algo(state):
    sample_fn = list(SAMPLING_FUNCTIONS.values())[state.range(0)]
    algo = list(TB_ALGOS.values())[state.range(1)]

    local_sets = gen_uniq_rng_set(state.range(3), state.range(2))
    local_sample = sample_fn(local_sets, state.range(4))

    arg_sets = dispy.create_subsets(local_sample)
    while state:
        algo(arg_sets)


if __name__ == "__main__":
    benchmark.main()
