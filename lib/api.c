// SPDX-License-Identifier: LGPL
/* Copyright (C) 2018-2019 Jiaxun Yang <jiaxun.yang@flygoat.com> */
/* RyzenAdj API */

#include "ryzenadj.h"

EXP ryzen_access CALL init_ryzenadj()
{
	smu_service_args_t args = {0, 0, 0, 0, 0, 0};
	ryzen_access ry;
	enum ryzen_family family = cpuid_get_family();
	if (family == FAM_UNKNOWN)
		return NULL;

	ry = (ryzen_access)malloc((sizeof(*ry)));

	ry->family = family;

	ry->pci_obj = init_pci_obj();
	if(!ry->pci_obj){
		printf("Unable to get PCI Obj\n");
		return NULL;
	}

	ry->nb = get_nb(ry->pci_obj);
	if(!ry->nb){
		printf("Unable to get NB Obj\n");
		goto out_free_pci_obj;
	}

	ry->mp1_smu = get_smu(ry->nb, TYPE_MP1);
	if(!ry->mp1_smu){
		goto out_free_nb;
	}

	ry->psmu = get_smu(ry->nb, TYPE_PSMU);
	if(!ry->psmu){
		goto out_free_mp1_smu;
	}

	smu_service_req(ry->mp1_smu, 0x3, &args);
	ry->bios_if_ver = args.arg0;
	if(ry->bios_if_ver < 0x5){
		printf("Not a Ryzen NB SMU, BIOS Interface Ver: 0x%x", ry->bios_if_ver);
		goto out_err;
	}

	return ry;

out_err:
	free_smu(ry->psmu);
out_free_mp1_smu:
	free_smu(ry->mp1_smu);
out_free_nb:
	free_nb(ry->nb);
out_free_pci_obj:
	free_pci_obj(ry->pci_obj);
	free(ry);
	return NULL;
}

EXP void CALL cleanup_ryzenadj(ryzen_access ry){
	if (ry == NULL)
	    return;

	free_smu(ry->psmu);
	free_smu(ry->mp1_smu);
	free_nb(ry->nb);
	free_pci_obj(ry->pci_obj);
	free(ry);
}

EXP enum ryzen_family get_cpu_family(ryzen_access ry)
{
	return ry->family;
}

EXP int get_bios_if_ver(ryzen_access ry)
{
	return ry->bios_if_ver;
}

#define _do_adjust(OPT) \
do{ \
		smu_service_args_t args = {0, 0, 0, 0, 0, 0};    \
		args.arg0 = value; \
		if(smu_service_req(ry->mp1_smu, OPT, &args) == 0x1){   \
			return 0;   \
		} else {    \
			return -1;  \
		} \
}while(0);



EXP int CALL set_stapm_limit(ryzen_access ry, uint32_t value){
	switch (ry->family)
	{
	case FAM_RAVEN:
	case FAM_PICASSO:
		_do_adjust(0x1a);
		break;
	case FAM_RENOIR:
		_do_adjust(0x14);
	}
}

EXP int CALL set_fast_limit(ryzen_access ry, uint32_t value){
	switch (ry->family)
	{
	case FAM_RAVEN:
	case FAM_PICASSO:
		_do_adjust(0x1b);
		break;
	case FAM_RENOIR:
		_do_adjust(0x15);
	}
}

EXP int CALL set_slow_limit(ryzen_access ry, uint32_t value){
	switch (ry->family)
	{
	case FAM_RAVEN:
	case FAM_PICASSO:
		_do_adjust(0x1c);
		break;
	case FAM_RENOIR:
		_do_adjust(0x16);
	}
}

EXP int CALL set_slow_time(ryzen_access ry, uint32_t value){
	switch (ry->family)
	{
	case FAM_RAVEN:
	case FAM_PICASSO:
		_do_adjust(0x1d);
		break;
	case FAM_RENOIR:
		_do_adjust(0x17);
	}
}

EXP int CALL set_stapm_time(ryzen_access ry, uint32_t value){
	switch (ry->family)
	{
	case FAM_RAVEN:
	case FAM_PICASSO:
		_do_adjust(0x1e);
		break;
	case FAM_RENOIR:
		_do_adjust(0x18);
	}
}

EXP int CALL set_tctl_temp(ryzen_access ry, uint32_t value){
	switch (ry->family)
	{
	case FAM_RAVEN:
	case FAM_PICASSO:
		_do_adjust(0x1f);
		break;
	case FAM_RENOIR:
		_do_adjust(0x19);
	}
}

EXP int CALL set_vrm_current(ryzen_access ry, uint32_t value){
	switch (ry->family)
	{
	case FAM_RAVEN:
	case FAM_PICASSO:
		_do_adjust(0x20);
		break;
	case FAM_RENOIR:
		_do_adjust(0x1a);
	}
}

EXP int CALL set_vrmsoc_current(ryzen_access ry, uint32_t value){
	switch (ry->family)
	{
	case FAM_RAVEN:
	case FAM_PICASSO:
		_do_adjust(0x21);
		break;
	case FAM_RENOIR:
		_do_adjust(0x1b);
	}
}

EXP int CALL set_vrmmax_current(ryzen_access ry, uint32_t value){
	switch (ry->family)
	{
	case FAM_RAVEN:
	case FAM_PICASSO:
		_do_adjust(0x22);
		break;
	case FAM_RENOIR:
		_do_adjust(0x1c);
	}
}

EXP int CALL set_vrmsocmax_current(ryzen_access ry, uint32_t value){
	switch (ry->family)
	{
	case FAM_RAVEN:
	case FAM_PICASSO:
		_do_adjust(0x23);
		break;
	case FAM_RENOIR:
		_do_adjust(0x1d);
	}
}

EXP int CALL set_psi0_current(ryzen_access ry, uint32_t value){
	switch (ry->family)
	{
	case FAM_RAVEN:
	case FAM_PICASSO:
		_do_adjust(0x24);
		break;
	case FAM_RENOIR:
		_do_adjust(0x1e);
	}
}

EXP int CALL set_psi0soc_current(ryzen_access ry, uint32_t value){
	switch (ry->family)
	{
	case FAM_RAVEN:
	case FAM_PICASSO:
		_do_adjust(0x25);
		break;
	case FAM_RENOIR:
		_do_adjust(0x20);
	}
}

EXP int CALL set_max_gfxclk_freq(ryzen_access ry, uint32_t value) {
	switch (ry->family)
	{
	case FAM_RAVEN:
	case FAM_PICASSO:
		_do_adjust(0x46);
		break;
	case FAM_RENOIR:
		break;
	}
}

EXP int CALL set_min_gfxclk_freq(ryzen_access ry, uint32_t value) {
	switch (ry->family)
	{
	case FAM_RAVEN:
	case FAM_PICASSO:
		_do_adjust(0x47);
		break;
	case FAM_RENOIR:
		break;
	}
}

EXP int CALL set_max_socclk_freq(ryzen_access ry, uint32_t value){
	switch (ry->family)
	{
	case FAM_RAVEN:
	case FAM_PICASSO:
		_do_adjust(0x48);
		break;
	case FAM_RENOIR:
		break;
	}
}

EXP int CALL set_min_socclk_freq(ryzen_access ry, uint32_t value){
	switch (ry->family)
	{
	case FAM_RAVEN:
	case FAM_PICASSO:
		_do_adjust(0x49); 
		break;
	case FAM_RENOIR:
		break;
	}
}

EXP int CALL set_max_fclk_freq(ryzen_access ry, uint32_t value){
	switch (ry->family)
	{
	case FAM_RAVEN:
	case FAM_PICASSO:
		_do_adjust(0x4A);
		break;
	case FAM_RENOIR:
		break;
	}
}

EXP int CALL set_min_fclk_freq(ryzen_access ry, uint32_t value){
	switch (ry->family)
	{
	case FAM_RAVEN:
	case FAM_PICASSO:
		_do_adjust(0x4B);
		break;
	case FAM_RENOIR:
		break;
	}
}

EXP int CALL set_max_vcn(ryzen_access ry, uint32_t value){
	switch (ry->family)
	{
	case FAM_RAVEN:
	case FAM_PICASSO:
		_do_adjust(0x4C);
		break;
	case FAM_RENOIR:
		break;
	}
}

EXP int CALL set_min_vcn(ryzen_access ry, uint32_t value){
	switch (ry->family)
	{
	case FAM_RAVEN:
	case FAM_PICASSO:
		_do_adjust(0x4D);
		break;
	case FAM_RENOIR:
		break;
	}
}

EXP int CALL set_max_lclk(ryzen_access ry, uint32_t value){
	switch (ry->family)
	{
	case FAM_RAVEN:
	case FAM_PICASSO:
		_do_adjust(0x4E);
		break;
	case FAM_RENOIR:
		break;
	}
}

EXP int CALL set_min_lclk(ryzen_access ry, uint32_t value){
	switch (ry->family)
	{
	case FAM_RAVEN:
	case FAM_PICASSO:
		_do_adjust(0x4F);
		break;
	case FAM_RENOIR:
		break;
	}
}

EXP int CALL set_prochot_deassertion_ramp(ryzen_access ry, uint32_t value) {
	switch (ry->family)
	{
	case FAM_RAVEN:
	case FAM_PICASSO:
		_do_adjust(0x26);
		break;
	case FAM_RENOIR:
		_do_adjust(0x20);
		break;
	}
}
