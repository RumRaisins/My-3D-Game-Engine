#include "EUtil.h"

namespace E3D {

	std::ofstream *GLogStream = NULL;
	bool InitLog(const EString&fileName) {
		if (GLogStream == NULL) {
			GLogStream = new std::ofstream();
			GLogStream->open(fileName.c_str());
			if (GLogStream == NULL) {
				return true;
			}
			return false;
		}
		return true;
	}
	void Log(const EChar *string, ...) {
		if (GLogStream == NULL) {
			return;
		}
		EChar buffer[256];
		//��֤���߶���Ϊ��
		if (!string || !GLogStream)
			return;
		va_list arglist;  //�ɱ�Ĳ����б�
		//������ջ�ϵĿɱ�Ĳ�����ӡ����ַ���
		va_start(arglist, string);
		vsprintf(buffer, string, arglist);
		va_end(arglist);

		EString info(buffer);
		//�������Ϣ
		*GLogStream << info << std::endl;
		GLogStream->flush();
	}
	void CloseLog() {
		GLogStream->close();
		SafeDelete(GLogStream); 
	}



	
}
