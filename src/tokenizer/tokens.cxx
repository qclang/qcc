#include<tokenizer/tokens.hxx>
#include<cstdint>
#include<vector>

std::istream* operator>>(std::istream *in, Token &t) {
	uint32_t name_len;
	in->read(reinterpret_cast<char*>(&name_len), sizeof(name_len));

	std::vector<char> buffer(name_len);
	in->read(buffer.data(), name_len);
	t.name.assign(buffer.data(), name_len);

	uint32_t fname_len;
        in->read(reinterpret_cast<char*>(&fname_len), sizeof(fname_len));

        std::vector<char> fbuffer(fname_len);
        in->read(fbuffer.data(), fname_len);
        t.file_name.assign(fbuffer.data(), fname_len);

	in->read(reinterpret_cast<char*>(&t.ttype), sizeof(t.ttype));
        in->read(reinterpret_cast<char*>(&t.line), sizeof(t.line));
        in->read(reinterpret_cast<char*>(&t.column), sizeof(t.column));
        in->read(reinterpret_cast<char*>(&t.startOffset), sizeof(t.startOffset));
        in->read(reinterpret_cast<char*>(&t.endOffset), sizeof(t.endOffset));
        in->read(reinterpret_cast<char*>(&t.isSynthetic), sizeof(t.isSynthetic));
	return in;
}

std::ostream* operator<<(std::ostream *out, Token &t) {
	uint32_t name_len = static_cast<uint32_t>(t.name.size());
	out->write(reinterpret_cast<const char*>(&name_len), sizeof(name_len));
	out->write(t.name.c_str(), name_len);

	uint32_t fname_len = static_cast<uint32_t>(t.file_name.size());
        out->write(reinterpret_cast<const char*>(&fname_len), sizeof(fname_len));
        out->write(t.file_name.c_str(), fname_len);

	out->write(reinterpret_cast<const char*>(&t.ttype), sizeof(t.ttype));
	out->write(reinterpret_cast<const char*>(&t.line), sizeof(t.line));
	out->write(reinterpret_cast<const char*>(&t.column), sizeof(t.column));
	out->write(reinterpret_cast<const char*>(&t.startOffset), sizeof(t.startOffset));
	out->write(reinterpret_cast<const char*>(&t.endOffset), sizeof(t.endOffset));
	out->write(reinterpret_cast<const char*>(&t.isSynthetic), sizeof(t.isSynthetic));
	return out;
}
