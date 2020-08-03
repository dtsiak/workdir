#include <array>
#include <functional>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "qboot/qboot.hpp"

namespace fs = qboot::fs;
namespace mp = qboot::mp;
using mp::real, mp::rational, mp::parse;
using qboot::Context, qboot::PolynomialProgram, qboot::BootstrapEquation, qboot::Sector;
using qboot::algebra::Vector;
using std::array, std::map, std::set, std::move, std::vector, std::string, std::unique_ptr;

template <class T>
using dict = map<string, T, std::less<>>;
using Op = qboot::PrimaryOperator;
using Eq = qboot::Equation;
constexpr auto ContinuousType = qboot::SectorType::Continuous;
constexpr auto Odd = qboot::algebra::FunctionSymmetry::Odd;
constexpr auto Even = qboot::algebra::FunctionSymmetry::Even;

static string name(const dict<rational>& deltas);
static BootstrapEquation create(const Context& c, const dict<rational>& deltas, uint32_t numax,
                                const set<uint32_t>& spins);



BootstrapEquation create(const Context& c, const dict<rational>& deltas, uint32_t numax, const set<uint32_t>& spins)
{
	dict<Op> ops;
	ops.emplace("s", Op(real(deltas.at("s")), 0, c));  // i[1]
	ops.emplace("v", Op(real(deltas.at("v")), 0, c));  // v[1]
	auto ext = [&ops](auto o1, auto o2, auto o3, auto o4) {
		return array{ops.at(o1), ops.at(o2), ops.at(o3), ops.at(o4)};
	};
	Op one{c};
	vector<Sector> secs;
	// you can add discrete sectors
	// for example, to add sector "hoge" whose size of the matrix is 4,
	//   secs.emplace_back("hoge", 4);
	// if you know OPE coefficients of the sector, for example, {1.2, 0.7, -0.1, 0.3},
	//   secs.emplace_back("hoge", 4, Vector{real("1.2"), real("0.7"), real("-0.1"), real("0.3")});
	secs.emplace_back("unit", 1, Vector{real(1)});
	// {beta[s, s, s][1], beta[v, v, s][1]}
	secs.emplace_back("scalar", 2);
	// you can customize spectrums,
	// example:
	//   customize
	//     Sector s("hoge", 2, ContinuousType);
	//     for (const auto& spin: spins)
	//         if (spin % 2 == 0) s.add_op(spin);
	//     secs.push_back(s);
	//   to
	//     Sector s("hoge", 2, ContinuousType);
	//     s.add_op(0, real("3"));    // first scalar: 3 <= delta (irrelevance)
	//     s.add_op(2);               // use unitarity bound for spin-2
	//     s.add_op(4, real("8.1"));  // first spin-4: 8.1 <= delta
	//     // sector "hoge" consists of even-spin operators, and we customized spin-0, 2, 4
	//     // use unitarity bound for other operators
	//     for (const auto& spin: spins)
	//         if (spin % 2 == 0 && spin >= 6) s.add_op(spin);
	//     secs.push_back(s);
	{
		// {beta[s, s, op][1], beta[v, v, op][1]}
		Sector s("(i[1], even)", 2, ContinuousType);
                s.add_op(0, real("3"));
		for (const auto& spin : spins)
			if (spin % 2 == 0 && spin >= 2) s.add_op(spin);
		secs.push_back(s);
	}
	{
		// {beta[v, v, op][1]}
		Sector s("(i[-1], odd)", 1, ContinuousType);
		for (const auto& spin : spins)
			if (spin % 2 == 1 ) s.add_op(spin);
		secs.push_back(s);
	}
	{
		// {beta[s, v, op][1]}
		Sector s("(v[1], even)", 1, ContinuousType);
                s.add_op(0, real("3"));
		for (const auto& spin : spins)
			if (spin % 2 == 0 && spin >= 2) s.add_op(spin);
		secs.push_back(s);
	}
	{
		// {beta[s, v, op][1]}
		Sector s("(v[1], odd)", 1, ContinuousType);
		for (const auto& spin : spins)
			if (spin % 2 == 1) s.add_op(spin);
		secs.push_back(s);
	}
	{
		// {beta[v, v, op][1]}
		Sector s("(v[2], even)", 1, ContinuousType);
		for (const auto& spin : spins)
			if (spin % 2 == 0) s.add_op(spin);
		secs.push_back(s);
	}
	// do not edit from here
	Vector<real> val(3);
	val[0] = ((real("1") / real("2")) * mp::pow(real("2"), (real("-1") / real("2"))));
	val[1] = (real("1") / real("2"));
	val[2] = (real("1") / real("4"));
	BootstrapEquation boot(c, secs, numax);
	{
		Eq eq(boot, Odd);
		eq.add("unit", 0, 0, one, ext("s", "s", "s", "s"));
		eq.add("scalar", 0, 0, ops.at("s"), ext("s", "s", "s", "s"));
		eq.add("(i[1], even)", 0, 0, ext("s", "s", "s", "s"));
		boot.add_equation(eq);
	}
	{
		Eq eq(boot, Odd);
		eq.add("unit", 0, 0, one, ext("s", "s", "v", "v"));
		eq.add("scalar", 0, 1, 2 * val[0], ops.at("s"), ext("s", "s", "v", "v"));
		eq.add("scalar", 1, 0, 2 * val[0], ops.at("s"), ext("s", "s", "v", "v"));
		eq.add("scalar", 1, 1, val[1], ops.at("v"), ext("s", "v", "v", "s"));
		eq.add("(i[1], even)", 0, 1, 2 * val[0], ext("s", "s", "v", "v"));
		eq.add("(i[1], even)", 1, 0, 2 * val[0], ext("s", "s", "v", "v"));
		eq.add("(v[1], even)", 0, 0, val[1], ext("s", "v", "v", "s"));
		eq.add("(v[1], odd)", 0, 0, -val[1], ext("s", "v", "v", "s"));
		boot.add_equation(eq);
	}
	{
		Eq eq(boot, Odd);
		eq.add("(i[-1], odd)", 0, 0, ext("v", "v", "v", "v"));
		eq.add("(v[2], even)", 0, 0, val[1], ext("v", "v", "v", "v"));
		boot.add_equation(eq);
	}
	{
		Eq eq(boot, Odd);
		eq.add("unit", 0, 0, one, ext("v", "v", "v", "v"));
		eq.add("scalar", 1, 1, val[1], ops.at("s"), ext("v", "v", "v", "v"));
		eq.add("(i[1], even)", 1, 1, val[1], ext("v", "v", "v", "v"));
		eq.add("(v[2], even)", 0, 0, val[2], ext("v", "v", "v", "v"));
		boot.add_equation(eq);
	}
	{
		Eq eq(boot, Even);
		eq.add("unit", 0, 0, one, ext("s", "s", "v", "v"));
		eq.add("scalar", 0, 1, 2 * val[0], ops.at("s"), ext("s", "s", "v", "v"));
		eq.add("scalar", 1, 0, 2 * val[0], ops.at("s"), ext("s", "s", "v", "v"));
		eq.add("scalar", 1, 1, -val[1], ops.at("v"), ext("s", "v", "v", "s"));
		eq.add("(i[1], even)", 0, 1, 2 * val[0], ext("s", "s", "v", "v"));
		eq.add("(i[1], even)", 1, 0, 2 * val[0], ext("s", "s", "v", "v"));
		eq.add("(v[1], even)", 0, 0, -val[1], ext("s", "v", "v", "s"));
		eq.add("(v[1], odd)", 0, 0, val[1], ext("s", "v", "v", "s"));
		boot.add_equation(eq);
	}
	{
		Eq eq(boot, Even);
		eq.add("unit", 0, 0, one, ext("v", "v", "v", "v"));
		eq.add("scalar", 1, 1, val[1], ops.at("s"), ext("v", "v", "v", "v"));
		eq.add("(i[1], even)", 1, 1, val[1], ext("v", "v", "v", "v"));
		eq.add("(i[-1], odd)", 0, 0, val[1], ext("v", "v", "v", "v"));
		eq.add("(v[2], even)", 0, 0, -val[1], ext("v", "v", "v", "v"));
		boot.add_equation(eq);
	}
	{
		Eq eq(boot, Odd);
		eq.add("scalar", 1, 1, ops.at("v"), ext("s", "v", "s", "v"));
		eq.add("(v[1], even)", 0, 0, ext("s", "v", "s", "v"));
		eq.add("(v[1], odd)", 0, 0, ext("s", "v", "s", "v"));
		boot.add_equation(eq);
	}
	boot.finish();
	return boot;
}

int main(int argc, char* argv[])
{
	// internal precision (in binary digits)
	mp::global_prec = 1000;
	mp::global_rnd = MPFR_RNDN;
	// n_Max: the order of taylor expansion of gBlock (we recommend n_Max >= 0.4 * global_prec)
	// lambda: controls the number of derivatives (z = x + sqrt(y), (der x) ^ m (der y) ^ n for m + 2 n <= lambda)
	// dim: the dimension of the physical space
	// numax: controls the number of poles picked in the gBlock (numax + min(spin, numax) / 2)
	// parallel: the number of internal threads
	constexpr uint32_t n_Max = 400, lambda = 14, numax = 6, parallel = 4;
	const rational dim("3");
	// spins: spins for the continuous sectors
	set<uint32_t> spins;
	for (uint32_t s = 0; s < 27; ++s) spins.insert(s);
	spins.merge(set{49u, 50u});
	assert(argc == 4);
	unique_ptr<char*[]> args(argv);
	dict<rational> deltas;
	// external scalars
	deltas["s"] = parse(args[1]).value();
	deltas["v"] = parse(args[2]).value();
	args.release();
	Context c(n_Max, lambda, dim, parallel);
	auto boot = create(c, deltas, numax, spins);
	// to maximize (resp. minimize) OPE in "hoge" sector,
	// replace the first argument from qboot::FindContradiction(...)
	// to qboot::ExtremalOPE(true (resp. false), "hoge", "unit")
	auto prob = boot.convert(qboot::FindContradiction("unit"), parallel);
        std::string dphi_string(argv[1]);
        std::string dS_string(argv[2]);
        std::string dY_string("1.2");
        std::string dir_name = string("Cubic_phi-S_mixed_Y_grid-dext") + dphi_string + "-dS" + dS_string + "-Dmin" + dY_string;
        //std::string dir_name = string("Cubic_phi-S_mixed_S_grid_XY1.19-1.23_phip1.97-2.13_T3_Xp3_Yp3_Sp3_phipp3_Tp4_A2.001_Lambda35-dext") + dphi_string + "-Dmin" + dS_string;
        auto dir = fs::current_path() / dir_name;
	move(prob).create_input(parallel).write(dir, parallel);
	return 0;
}
