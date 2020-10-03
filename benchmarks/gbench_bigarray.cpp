#include "benchmark/benchmark.h"
#include <nctl/Array.h>
#define TEST_WITH_NCTL
#include "test_movable.h"

const unsigned int Capacity = 1024;

static void BM_BigArrayCreation(benchmark::State &state)
{
	for (auto _ : state)
	{
		nctl::Array<Movable> array(state.range(0));
		benchmark::DoNotOptimize(array);
	}
}
BENCHMARK(BM_BigArrayCreation)->Arg(Capacity / 4)->Arg(Capacity / 2)->Arg(Capacity);

static void BM_BigArrayCopy(benchmark::State &state)
{
	nctl::Array<Movable> initArray(state.range(0));
	for (unsigned int i = 0; i < state.range(0); i++)
		initArray.pushBack(Movable(Movable::Construction::INITIALIZED));
	nctl::Array<Movable> array(state.range(0));

	for (auto _ : state)
	{
		array = initArray;
		benchmark::DoNotOptimize(array);

		state.PauseTiming();
		array.clear();
		state.ResumeTiming();
	}
}
BENCHMARK(BM_BigArrayCopy)->Arg(Capacity / 4)->Arg(Capacity / 2)->Arg(Capacity);

static void BM_BigArrayMove(benchmark::State &state)
{
	nctl::Array<Movable> initArray(state.range(0));
	for (unsigned int i = 0; i < state.range(0); i++)
		initArray.pushBack(Movable(Movable::Construction::INITIALIZED));
	nctl::Array<Movable> array(state.range(0));

	for (auto _ : state)
	{
		array = nctl::move(initArray);
		benchmark::DoNotOptimize(array);

		state.PauseTiming();
		array.clear();
		state.ResumeTiming();
	}
}
BENCHMARK(BM_BigArrayMove)->Arg(Capacity / 4)->Arg(Capacity / 2)->Arg(Capacity);

static void BM_BigArrayPushBack(benchmark::State &state)
{
	nctl::Array<Movable> array(state.range(0));

	for (auto _ : state)
	{
		for (unsigned int i = 0; i < state.range(0); i++)
		{
			Movable movable(Movable::Construction::INITIALIZED);
			array.pushBack(movable);
			benchmark::DoNotOptimize(array);
		}

		state.PauseTiming();
		array.clear();
		state.ResumeTiming();
	}
}
BENCHMARK(BM_BigArrayPushBack)->Arg(Capacity / 4)->Arg(Capacity / 2)->Arg(Capacity);

static void BM_BigArrayMovePushBack(benchmark::State &state)
{
	nctl::Array<Movable> array(state.range(0));
	Movable movable(Movable::Construction::INITIALIZED);

	for (auto _ : state)
	{
		for (unsigned int i = 0; i < state.range(0); i++)
		{
			Movable movable(Movable::Construction::INITIALIZED);
			array.pushBack(nctl::move(movable));
			benchmark::DoNotOptimize(array);
		}

		state.PauseTiming();
		array.clear();
		state.ResumeTiming();
	}
}
BENCHMARK(BM_BigArrayMovePushBack)->Arg(Capacity / 4)->Arg(Capacity / 2)->Arg(Capacity);

static void BM_BigArrayEmplaceBack(benchmark::State &state)
{
	nctl::Array<Movable> array(state.range(0));

	for (auto _ : state)
	{
		for (unsigned int i = 0; i < state.range(0); i++)
		{
			array.emplaceBack(Movable::Construction::INITIALIZED);
			benchmark::DoNotOptimize(array);
		}

		state.PauseTiming();
		array.clear();
		state.ResumeTiming();
	}
}
BENCHMARK(BM_BigArrayEmplaceBack)->Arg(Capacity / 4)->Arg(Capacity / 2)->Arg(Capacity);

static void BM_BigArrayClear(benchmark::State &state)
{
	nctl::Array<Movable> initArray(state.range(0));
	for (unsigned int i = 0; i < state.range(0); i++)
		initArray.pushBack(Movable(Movable::Construction::INITIALIZED));

	for (auto _ : state)
	{
		state.PauseTiming();
		nctl::Array<Movable> array(initArray);
		state.ResumeTiming();

		array.clear();
	}
}
BENCHMARK(BM_BigArrayClear)->Arg(Capacity / 4)->Arg(Capacity / 2)->Arg(Capacity);

static void BM_BigArrayErase(benchmark::State &state)
{
	nctl::Array<Movable> initArray(state.range(0));
	for (unsigned int i = 0; i < state.range(0); i++)
		initArray.pushBack(Movable(Movable::Construction::INITIALIZED));

	for (auto _ : state)
	{
		state.PauseTiming();
		nctl::Array<Movable> array(initArray);
		state.ResumeTiming();

		for (unsigned int i = 0; i < state.range(0); i++)
			benchmark::DoNotOptimize(array.erase(array.end() - 1));
	}
}
BENCHMARK(BM_BigArrayErase)->Arg(Capacity / 4)->Arg(Capacity / 2)->Arg(Capacity);

static void BM_BigArrayReverseErase(benchmark::State &state)
{
	nctl::Array<Movable> initArray(state.range(0));
	for (unsigned int i = 0; i < state.range(0); i++)
		initArray.pushBack(Movable(Movable::Construction::INITIALIZED));

	for (auto _ : state)
	{
		state.PauseTiming();
		nctl::Array<Movable> array(initArray);
		state.ResumeTiming();

		for (int i = state.range(0) - 1; i >= 0; i--)
			benchmark::DoNotOptimize(array.erase(array.begin()));
	}
}
BENCHMARK(BM_BigArrayReverseErase)->Arg(Capacity / 4)->Arg(Capacity / 2)->Arg(Capacity);

BENCHMARK_MAIN();
