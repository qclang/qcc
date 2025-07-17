namespace Variables {

struct Type {
	Type *parent;
};

//primitives
namespace Prim {
	constexpr Type  t_bit{},
			t_qbit{},
			t_int{},
			t_float{},
			t_angle{};
}

}
