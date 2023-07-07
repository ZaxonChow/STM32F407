#include "bsp_rcc.h"

#define PLL_M      25
#define PLL_P      2
#define PLL_Q      7
#define PLL_N      336

void User_SetSysClock(void)
{
  RCC_DeInit();
/******************************************************************************/
/*            PLL (clocked by HSE) used as System clock source                */
/******************************************************************************/
  __IO uint32_t StartUpCounter = 0, HSEStatus = 0;
  
  /* Enable HSE */
  /* ʹ��HSE */
  RCC->CR |= ((uint32_t)RCC_CR_HSEON);
 
  /* Wait till HSE is ready and if Time out is reached exit */
  /* �ȴ�HSE�ȶ������������ʱ�����˳� */
  do
  {
    HSEStatus = RCC->CR & RCC_CR_HSERDY;
    StartUpCounter++;
  } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

  if ((RCC->CR & RCC_CR_HSERDY) != RESET)
  {
    HSEStatus = (uint32_t)0x01;
  }
  else
  {
    HSEStatus = (uint32_t)0x00;
  }
  
  /* HSE �����ɹ� */
  if (HSEStatus == (uint32_t)0x01)
  {
    /* Select regulator voltage output Scale 1 mode */
	/* ѡ���ѹ��������ģʽΪ1 */
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    PWR->CR |= PWR_CR_VOS;

    /* HCLK = SYSCLK / 1*/
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

    /* PCLK2 = HCLK / 2*/
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;
    
    /* PCLK1 = HCLK / 4*/
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;

    /* Configure the main PLL */
	/* ������PLL */
    RCC->PLLCFGR = PLL_M | (PLL_N << 6) | (((PLL_P >> 1) -1) << 16) |
                   (RCC_PLLCFGR_PLLSRC_HSE) | (PLL_Q << 24);

    /* Enable the main PLL */
	/* ʹ����PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* Wait till the main PLL is ready */
	/* �ȴ���PLL�ȶ� */
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }
   
    /* Configure Flash prefetch, Instruction cache, Data cache and wait state */
	/* ����FLASHԤȡֵ��ָ��棬���ݻ��棬�ȴ�����*/
    FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_5WS;

    /* Select the main PLL as system clock source */
	/* ѡ����PLLʱ����Ϊϵͳʱ��*/
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= RCC_CFGR_SW_PLL;

    /* Wait till the main PLL is used as system clock source */
	/* ȷ����PLLʱ��ѡΪϵͳʱ��*/
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
    {
    }
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock
         configuration. User can add here some code to deal with this error */
	/* HSE����ʧ�ܣ��������������ʧ�ܵĳ��� */
  }
}

void HSE_SetSysClock(uint32_t PLLM, uint32_t PLLN, uint32_t PLLP, uint32_t PLLQ)
{
	ErrorStatus HSE_ErrorStatus = ERROR;
	
	//��λRCC�����мĴ���
	RCC_DeInit();
	
	/* ʹ��HSE */
	RCC_HSEConfig(RCC_HSE_ON);
	
	/* �ȴ�HSE�ȶ������������ʱ�����˳� */
	HSE_ErrorStatus = RCC_WaitForHSEStartUp();
	if(HSE_ErrorStatus == SUCCESS)
	{
		/* ѡ���ѹ��������ģʽΪ1 */
		RCC->APB1ENR |= RCC_APB1ENR_PWREN;
		PWR->CR |= PWR_CR_VOS;
		
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		RCC_PCLK1Config(RCC_HCLK_Div4);
		RCC_PCLK2Config(RCC_HCLK_Div2);
		
		/* ������PLL */		
		RCC_PLLConfig(RCC_PLLSource_HSE, PLLM, PLLN, PLLP, PLLQ);
		
		/* ʹ����PLL */
		RCC_PLLCmd(ENABLE);

		/* �ȴ���PLL�ȶ� */
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
		{
		}
		
		/* ����FLASHԤȡֵ��ָ��棬���ݻ��棬�ȴ�����*/
		FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_5WS;
		
		/* ѡ����PLLʱ����Ϊϵͳʱ��*/
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		
		/* ȷ����PLLʱ��ѡΪϵͳʱ��*/
		while (RCC_GetSYSCLKSource() != 0x08)
		{
		}
	}
	else
	{
		/* HSE����ʧ�ܣ���Ӵ�������� */
	}

}

void HSI_SetSysClock(uint32_t PLLM, uint32_t PLLN, uint32_t PLLP, uint32_t PLLQ)
{
	volatile uint32_t HSI_ErrorStatus = 0;
	
	//��λRCC�����мĴ���
	RCC_DeInit();
	
	/* ʹ��HSI */
	RCC_HSICmd(ENABLE);
	
	/* �ȴ�HSI�ȶ������������ʱ�����˳� */
	HSI_ErrorStatus = RCC->CR & RCC_CR_HSIRDY;
	if(HSI_ErrorStatus == RCC_CR_HSIRDY)
	{
		/* ѡ���ѹ��������ģʽΪ1 */
		RCC->APB1ENR |= RCC_APB1ENR_PWREN;
		PWR->CR |= PWR_CR_VOS;
		
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		RCC_PCLK1Config(RCC_HCLK_Div4);
		RCC_PCLK2Config(RCC_HCLK_Div2);
		
		/* ������PLL */		
		RCC_PLLConfig(RCC_PLLSource_HSI, PLLM, PLLN, PLLP, PLLQ);
		
		/* ʹ����PLL */
		RCC_PLLCmd(ENABLE);

		/* �ȴ���PLL�ȶ� */
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
		{
		}
		
		/* ����FLASHԤȡֵ��ָ��棬���ݻ��棬�ȴ�����*/
		FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_5WS;
		
		/* ѡ����PLLʱ����Ϊϵͳʱ��*/
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		
		/* ȷ����PLLʱ��ѡΪϵͳʱ��*/
		while (RCC_GetSYSCLKSource() != 0x08)
		{
		}
	}
	else
	{
		/* HSE����ʧ�ܣ���Ӵ�������� */
	}

}

void MCO1_GPIO_Config(void)
{
	/*��һ������GPIO��ʱ��*/
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE);
	
	/*�ڶ���������һ��GPIO��ʼ���ṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*������������GPIO��ʼ���ṹ���Ա*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	/*���Ĳ�������GPIO��ʼ�������������úõĽṹ���Ա����д��Ĵ���*/
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void MCO2_GPIO_Config(void)
{
	/*��һ������GPIO��ʱ��*/
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOC, ENABLE);
	
	/*�ڶ���������һ��GPIO��ʼ���ṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*������������GPIO��ʼ���ṹ���Ա*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	/*���Ĳ�������GPIO��ʼ�������������úõĽṹ���Ա����д��Ĵ���*/
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
