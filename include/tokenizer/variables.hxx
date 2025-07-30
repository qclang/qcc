namespace Variables {

struct Type {
	const Type *parent = nullptr;
};

//primitives
namespace Prim {
	inline const Type  t_bit{},
			t_qbit{},
			t_int{},
			t_float{},
			t_angle{};
}

}
