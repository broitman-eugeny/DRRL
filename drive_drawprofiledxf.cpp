//��������� ������� ��������� ��� � ����������� ���� � ������� DXF
extern "C" bool DrawProfileDXF(char* profile001_path_file_name, //���� � ��� ����� ������� ������� 001
						 char* profileDXF_path_file_name, //���� � ��� ��������� ������������ DXF ����� �������
                   double Gradient_d,               //�������� ��������������� ������������� �������, 1/��10-8
                   double Chast_d,                  //������� �������, ���
                   double Pogr_d,                   //����������� ������� �����, �
                   double Hosnleft_d,               //������ ������ �������� �������� ������� �����, �
                   double Hosnright_d,              //������ ������ �������� �������� ������� ������, �
                   int PRP,                         //���� ������� ���������������-������������ ������: 0 - ��� ���, 1 - ��� ����
                   double HdopLeft_d,               //������ ������ �������� �������������� ������� �����, �
                   double HdopRight_d);              //������ ������ �������� �������������� ������� ������, �

void main()
{

	DrawProfileDXF("D:\\work\\Mwork\\DRRL_quick\\����\\90 �.001", //���� � ��� ����� ������� ������� 001
						 "D:\\work\\Mwork\\DRRL_quick\\����\\90 �.dxf", //���� � ��� ��������� ������������ DXF ����� �������
                   -10e-8,               //�������� ��������������� ������������� �������, 1/�
                   8.,                  //������� �������, ���
                   3.,                   //����������� ������� �����, �
                   30.,               //������ ������ �������� �������� ������� �����, �
                   30.,              //������ ������ �������� �������� ������� ������, �
                   0.,                         //���� ������� ���������������-������������ ������: 0 - ��� ���, 1 - ��� ����
                   20.,               //������ ������ �������� �������������� ������� �����, �
                   20.);              //������ ������ �������� �������������� ������� ������, �


}