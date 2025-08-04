#include<tokenizer/tokens.hxx>
#include<cstdint>

std::istream* operator>>(std::istream *in, Token &t) {
	uint32_t name_len;
	in->read(reinterpret_cast<char*>(&name_len), sizeof(name_len));
	char name[name_len];
	in->read(name, name_len);
	t.name = name;

	in->read(reinterpret_cast<char*>(&t.ttype), sizeof(t.ttype));
        in->read(reinterpret_cast<char*>(&t.line), sizeof(t.line));
        in->read(reinterpret_cast<char*>(&t.column), sizeof(t.column));
        in->read(reinterpret_cast<char*>(&t.startOffset), sizeof(t.startOffset));
        in->read(reinterpret_cast<char*>(&t.endOffset), sizeof(t.endOffset));
	return in;
}

std::ostream* operator<<(std::ostream *out, Token &t) {
	uint32_t name_len = static_cast<uint32_t>(t.name.size());
	out->write(reinterpret_cast<const char*>(&name_len), sizeof(name_len));
	out->write(t.name.c_str(), name_len);

	out->write(reinterpret_cast<const char*>(&t.ttype), sizeof(t.ttype));
	out->write(reinterpret_cast<const char*>(&t.line), sizeof(t.line));
	out->write(reinterpret_cast<const char*>(&t.column), sizeof(t.column));
	out->write(reinterpret_cast<const char*>(&t.startOffset), sizeof(t.startOffset));
	out->write(reinterpret_cast<const char*>(&t.endOffset), sizeof(t.endOffset));
	return out;
}
