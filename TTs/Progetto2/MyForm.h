#pragma once
#include <windows.h>
#include <mmsystem.h>
#include <msclr/marshal_cppstd.h>
#define AUDIOBYTES 40000

namespace Progetto2 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;
	using namespace System::Text;


	/// <summary>
	/// Riepilogo per MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: aggiungere qui il codice del costruttore.
			//
		}

	protected:
		/// <summary>
		/// Pulire le risorse in uso.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Button^  button1;
	protected:

	private:
		/// <summary>
		/// Variabile di progettazione necessaria.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Metodo necessario per il supporto della finestra di progettazione. Non modificare
		/// il contenuto del metodo con l'editor di codice.
		/// </summary>
		void InitializeComponent(void)
		{
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(90, 46);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(100, 20);
			this->textBox1->TabIndex = 0;
			this->textBox1->Text = L"What\'s your name\?";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(119, 163);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(0, 13);
			this->label1->TabIndex = 1;
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(101, 96);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 2;
			this->button1->Text = L"Done";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(284, 261);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->textBox1);
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			this->ResumeLayout(false);
			this->PerformLayout();

		}

	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {

		/*MemoryStream^ memStream = gcnew MemoryStream(1000);
		memStream = TextToSpeechHandler::TextToSpeech(this->textBox1->Text);
		memStream->Position = 0;
		FileStream ^fs = File::OpenWrite("file.txt");
		memStream->WriteTo(fs);

		memStream->Close();
		fs->Close();
		*/
		

		int size = this->textBox1->Text->Length;

		interior_ptr<const Char> ppchar = PtrToStringChars(this->textBox1->Text);

		Char* str = (Char*)malloc(sizeof(Char)*size);

		unsigned char* audio = (unsigned char*)malloc(size*AUDIOBYTES);

		for (int i = 0; i < size; i++)
			str[i] = *(ppchar + i);

		int audiolength = TTSWrap::TTSWrap::TextToPCM(str,&audio[0],size);

		FileStream ^fs = File::OpenWrite("file.txt");
		
		int count = 0;

		while (count < audiolength)
		{
			fs->WriteByte(audio[count++]);
		}

		fs->Close();

		free(str);
		free(audio);
	}

	}
	; }
