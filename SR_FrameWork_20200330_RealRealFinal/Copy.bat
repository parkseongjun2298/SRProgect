// 명령어			옵션			복사할 경로						복사받을 경로
xcopy			/y			.\Engine\Headers\*.h			.\Reference\Headers\

xcopy			/y			.\Engine\Systems\Codes\*.h		.\Reference\Headers\
xcopy			/y			.\Engine\Systems\Bin\*.lib		.\Reference\Libraries\
xcopy			/y			.\Engine\Systems\Bin\*.dll		.\Client\Bin\

xcopy			/y			.\Engine\Utilities\Codes\*.h	.\Reference\Headers\
xcopy			/y			.\Engine\Utilities\Bin\*.lib	.\Reference\Libraries\
xcopy			/y			.\Engine\Utilities\Bin\*.dll	.\Client\Bin\

xcopy			/y			.\Engine\Resources\Codes\*.h	.\Reference\Headers\
xcopy			/y			.\Engine\Resources\Bin\*.lib	.\Reference\Libraries\
xcopy			/y			.\Engine\Resources\Bin\*.dll	.\Client\Bin\