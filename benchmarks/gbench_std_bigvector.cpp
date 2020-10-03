#include "benchmark/benchmark.h"
#include <vector>
#include "test_movable.h"

const unsigned int Capacity = 1024;

static void BM_BigVectorCreation(benchmark::State &state)
{
	for (auto _ : state)
	{
		std::vector<Movable> vector(state.range(0));
		benchmark::DoNotOptimize(vector);
	}
}
BENCHMARK(BM_BigVectorCreation)->Arg(Capacity / 4)->Arg(Capacity / 2)->Arg(Capacity);

static void BM_BigVectorCopy(benchmark::State &state)
{
	std::vector<Movable> initVector(state.range(0));
	for (unsigned int i = 0; i < state.range(0); i++)
		initVector.push_back(Movable(Movable::Construction::INITIALIZED));
	std::vector<Movable> vector(state.range(0));

	for (auto _ : state)
	{
		vector = initVector;
		benchmark::DoNotOptimize(vector);

		state.PauseTiming();
		vector.clear();
		state.ResumeTiming();
	}
}
BENCHMARK(BM_BigVectorCopy)->Arg(Capacity / 4)->Arg(Capacity / 2)->Arg(Capacity);

static void BM_BigVectorMove(benchmark::State &state)
{
	std::vector<Movable> initVector(state.range(0));
	for (unsigned int i = 0; i < state.range(0); i++)
		initVector.push_back(Movable(Movable::Construction::INITIALIZED));
	std::vector<Movable> vector(state.range(0));

	for (auto _ : state)
	{
		vector = std::move(initVector);
		benchmark::DoNotOptimize(vector);

		state.PauseTiming();
		vector.clear();
		state.ResumeTiming();
	}
}
BENCHMARK(BM_BigVectorMove)->Arg(Capacity / 4)->Arg(Capacity / 2)->Arg(Capacity);

static void BM_BigVectorPushBack(benchmark::State &state)
{
	std::vector<Movable> vector(state.range(0));

	for (auto _ : state)
	{
		for (unsigned int i = 0; i < state.range(0); i++)
		{
			Movable movable(Movable::Construction::INITIALIZED);
			vector.push_back(movable);
			benchmark::DoNotOptimize(vector);
		}

		state.PauseTiming();
		vector.clear();
		state.ResumeTiming();
	}
}
BENCHMARK(BM_BigVectorPushBack)->Arg(Capacity / 4)->Arg(Capacity / 2)->Arg(Capacity);

static void BM_BigVectorMovePushBack(benchmark::State &state)
{
	std::vector<Movable> vector(state.range(0));
	Movable movable(Movable::Construction::INITIALIZED);

	for (auto _ : state)
	{
		for (unsigned int i = 0; i < state.range(0); i++)
		{
			Movable movable(Movable::Construction::INITIALIZED);
			vector.push_back(std::move(movable));
			benchmark::DoNotOptimize(vector);
		}

		state.PauseTiming();
		vector.clear();
		state.ResumeTiming();
	}
}
BENCHMARK(BM_BigVectorMovePushBack)->Arg(Capacity / 4)->Arg(Capacity / 2)->Arg(Capacity);

static void BM_BigVectorEmplaceBack(benchmark::State &state)
{
	std::vector<Movable> vector(state.range(0));

	for (auto _ : state)
	{
		for (unsigned int i = 0; i < state.range(0); i++)
		{
			vector.emplace_back(Movable::Construction::INITIALIZED);
			benchmark::DoNotOptimize(vector);
		}

		state.PauseTiming();
		vector.clear();
		state.ResumeTiming();
	}
}
BENCHMARK(BM_BigVectorEmplaceBack)->Arg(Capacity / 4)->Arg(Capacity / 2)->Arg(Capacity);

static void BM_BigVectorClear(benchmark::State &state)
{
	std::vector<Movable> initVector(state.range(0));
	for (unsigned int i = 0; i < state.range(0); i++)
		initVector.push_back(Movable(Movable::Construction::INITIALIZED));

	for (auto _ : state)
	{
		state.PauseTiming();
		std::vector<Movable> vector(initVector);
		state.ResumeTiming();

		vector.clear();
	}
}
BENCHMARK(BM_BigVectorClear)->Arg(Capacity / 4)->Arg(Capacity / 2)->Arg(Capacity);

static void BM_BigVectorErase(benchmark::State &state)
{
	std::vector<Movable> initVector(state.range(0));
	for (unsigned int i = 0; i < state.range(0); i++)
		initVector.push_back(Movable(Movable::Construction::INITIALIZED));

	for (auto _ : state)
	{
		state.PauseTiming();
		std::vector<Movable> vector(initVector);
		state.ResumeTiming();

		for (unsigned int i = 0; i < state.range(0); i++)
			benchmark::DoNotOptimize(vector.erase(vector.end() - 1));
	}
}
BENCHMARK(BM_BigVectorErase)->Arg(Capacity / 4)->Arg(Capacity / 2)->Arg(Capacity);

static void BM_BigVectorReverseErase(benchmark::State &state)
{
	std::vector<Movable> initVector(state.range(0));
	for (unsigned int i = 0; i < state.range(0); i++)
		initVector.push_back(Movable(Movable::Construction::INITIALIZED));

	for (auto _ : state)
	{
		state.PauseTiming();
		std::vector<Movable> vector(initVector);
		state.ResumeTiming();

		for (int i = state.range(0) - 1; i >= 0; i--)
			benchmark::DoNotOptimize(vector.erase(vector.begin()));
	}
}
BENCHMARK(BM_BigVectorReverseErase)->Arg(Capacity / 4)->Arg(Capacity / 2)->Arg(Capacity);

BENCHMARK_MAIN();
