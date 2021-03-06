/*
// Copyright (c) 2012-2015 Pierre Guillot, Eliott Paris & Thomas Le Meur CICM, Universite Paris 8.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "../hoa.library.hpp"
#include "../ThirdParty/HoaLibrary/Sources/Hoa.hpp"
using namespace hoa;

typedef struct _hoa_optim
{
    t_edspobj               f_obj;
    Optim<Hoa2d, t_sample>* f_optim;
    t_sample*               f_ins;
    t_sample*               f_outs;
    t_symbol*               f_mode;
} t_hoa_optim;

static t_eclass *hoa_optim_class;

typedef struct _hoa_optim_3d
{
    t_edspobj               f_obj;
    Optim<Hoa3d, t_sample>* f_optim;
    t_sample*               f_ins;
    t_sample*               f_outs;
    t_symbol*               f_mode;
} t_hoa_optim_3d;

static t_eclass *hoa_optim_3d_class;

static void hoa_optim_perform_basic(t_hoa_optim *x, t_object *dsp64, t_sample **ins, long numins, t_sample **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    for(long i = 0; i < numins; i++)
    {
        Signal<t_sample>::copy(ulong(sampleframes), ins[i], 1, x->f_ins+i, ulong(numins));
    }
    for(long i = 0; i < numouts; i++)
    {
        Signal<t_sample>::copy(ulong(sampleframes), x->f_ins+i, ulong(numouts), outs[i], 1);
    }
}

static void hoa_optim_perform_maxRe(t_hoa_optim *x, t_object *dsp64, t_sample **ins, long numins, t_sample **outs, long numouts, long sampleframes, long flags, void *userparam)
{
	for(long i = 0; i < numins; i++)
    {
        Signal<t_sample>::copy(ulong(sampleframes), ins[i], 1, x->f_ins+i, ulong(numins));
    }
	for(long i = 0; i < sampleframes; i++)
    {
        (static_cast<Optim<Hoa2d, t_sample>::MaxRe *>(x->f_optim))->process(x->f_ins + numins * i, x->f_outs + numouts * i);
    }
    for(long i = 0; i < numouts; i++)
    {
        Signal<t_sample>::copy(ulong(sampleframes), x->f_outs+i, ulong(numouts), outs[i], 1);
    }
}

static void hoa_optim_perform_inPhase(t_hoa_optim *x, t_object *dsp64, t_sample **ins, long numins, t_sample **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    for(long i = 0; i < numins; i++)
    {
        Signal<t_sample>::copy(ulong(sampleframes), ins[i], 1, x->f_ins+i, ulong(numins));
    }
    for(long i = 0; i < sampleframes; i++)
    {
        (static_cast<Optim<Hoa2d, t_sample>::InPhase *>(x->f_optim))->process(x->f_ins + numins * i, x->f_outs + numouts * i);
    }
    for(long i = 0; i < numouts; i++)
    {
        Signal<t_sample>::copy(ulong(sampleframes), x->f_outs+i, ulong(numouts), outs[i], 1);
    }
}

static void hoa_optim_dsp(t_hoa_optim *x, t_object *dsp, short *count, double samplerate, long maxvectorsize, long flags)
{
    if(x->f_mode == hoa_sym_basic)
        object_method(dsp, gensym("dsp_add"), x, (method)hoa_optim_perform_basic, 0, NULL);
    else if(x->f_mode == hoa_sym_maxRe)
        object_method(dsp, gensym("dsp_add"), x, (method)hoa_optim_perform_maxRe, 0, NULL);
    else if(x->f_mode == hoa_sym_inPhase)
        object_method(dsp, gensym("dsp_add"), x, (method)hoa_optim_perform_inPhase, 0, NULL);
}

static void hoa_optim_symbol(t_hoa_optim *x, t_symbol* s)
{
    if(s != x->f_mode && (s == hoa_sym_basic || s == hoa_sym_maxRe || s == hoa_sym_inPhase))
    {
        int state = canvas_suspend_dsp();
        ulong  order = x->f_optim->getDecompositionOrder();
        x->f_mode = s;
        delete x->f_optim;
        if(x->f_mode == hoa_sym_basic)
        {
            x->f_optim = new Optim<Hoa2d, t_sample>::Basic(order);
        }
        else if(x->f_mode == hoa_sym_maxRe)
        {
            x->f_optim = new Optim<Hoa2d, t_sample>::MaxRe(order);
        }
        else
        {
            x->f_optim  = new Optim<Hoa2d, t_sample>::InPhase(order);
        }
        canvas_resume_dsp(state);
    }
}

static void *hoa_optim_new(t_symbol *s, int argc, t_atom *argv)
{
    ulong order       = 1;
    t_hoa_optim *x  = (t_hoa_optim *)eobj_new(hoa_optim_class);
    if(x)
    {
        x->f_mode   = hoa_sym_inPhase;
        if(atom_gettype(argv) == A_LONG)
        {
            order = ulong(pd_clip_minmax(atom_getlong(argv), 1, 63));
        }
        if(argc > 1 && atom_gettype(argv+1) == A_SYM && atom_getsymbol(argv+1) == hoa_sym_maxRe)
        {
            x->f_mode = hoa_sym_maxRe;
            x->f_optim = new Optim<Hoa2d, t_sample>::MaxRe(order);
        }
        else if(argc > 1 && atom_gettype(argv+1) == A_SYM && atom_getsymbol(argv+1) == hoa_sym_basic)
        {
            x->f_mode = hoa_sym_basic;
            x->f_optim = new Optim<Hoa2d, t_sample>::Basic(order);
        }
        else
        {
            x->f_optim  = new Optim<Hoa2d, t_sample>::InPhase(order);
        }
        
        eobj_dspsetup(x, long(x->f_optim->getNumberOfHarmonics()), long(x->f_optim->getNumberOfHarmonics()));
        x->f_ins = Signal<t_sample>::alloc(x->f_optim->getNumberOfHarmonics() * HOA_MAXBLKSIZE);
        x->f_outs = Signal<t_sample>::alloc(x->f_optim->getNumberOfHarmonics() * HOA_MAXBLKSIZE);
        
        return x;
    }
    
   	return NULL;
}

static void hoa_optim_free(t_hoa_optim *x)
{
	eobj_dspfree(x);
	delete x->f_optim;
    Signal<t_sample>::free(x->f_ins);
    Signal<t_sample>::free(x->f_outs);
}

extern "C" void setup_hoa0x2e2d0x2eoptim_tilde(void)
{
    t_eclass* c;
    
    c = eclass_new("hoa.2d.optim~", (method)hoa_optim_new, (method)hoa_optim_free, (short)sizeof(t_hoa_optim), 0L, A_GIMME, 0);
    class_addcreator((t_newmethod)hoa_optim_new, gensym("hoa.optim~"), A_GIMME, 0);
    
    eclass_dspinit(c);
    
    eclass_addmethod(c, (method)hoa_optim_dsp,      "dsp",      A_CANT, 0);
    eclass_addmethod(c, (method)hoa_optim_symbol,   "symbol",	A_SYM,  0);
    
    eclass_register(CLASS_OBJ, c);
    hoa_optim_class = c;
}

static void hoa_optim_3d_perform_basic(t_hoa_optim_3d *x, t_object *dsp64, t_sample **ins, long numins, t_sample **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    for(long i = 0; i < numins; i++)
    {
        Signal<t_sample>::copy(ulong(sampleframes), ins[i], 1, x->f_ins+i, ulong(numins));
    }
    for(long i = 0; i < numouts; i++)
    {
        Signal<t_sample>::copy(ulong(sampleframes), x->f_ins+i, ulong(numouts), outs[i], 1);
    }
}

static void hoa_optim_3d_perform_maxRe(t_hoa_optim_3d *x, t_object *dsp64, t_sample **ins, long numins, t_sample **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    for(long i = 0; i < numins; i++)
    {
        Signal<t_sample>::copy(ulong(sampleframes), ins[i], 1, x->f_ins+i, ulong(numins));
    }
    for(long i = 0; i < sampleframes; i++)
    {
        (static_cast<Optim<Hoa3d, t_sample>::MaxRe *>(x->f_optim))->process(x->f_ins + numins * i, x->f_outs + numouts * i);
    }
    for(long i = 0; i < numouts; i++)
    {
        Signal<t_sample>::copy(ulong(sampleframes), x->f_outs+i, ulong(numouts), outs[i], 1);
    }
}

static void hoa_optim_3d_perform_inPhase(t_hoa_optim_3d *x, t_object *dsp64, t_sample **ins, long numins, t_sample **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    for(long i = 0; i < numins; i++)
    {
        Signal<t_sample>::copy(ulong(sampleframes), ins[i], 1, x->f_ins+i, ulong(numins));
    }
    for(long i = 0; i < sampleframes; i++)
    {
        (static_cast<Optim<Hoa3d, t_sample>::InPhase *>(x->f_optim))->process(x->f_ins + numins * i, x->f_outs + numouts * i);
    }
    for(long i = 0; i < numouts; i++)
    {
        Signal<t_sample>::copy(ulong(sampleframes), x->f_outs+i, ulong(numouts), outs[i], 1);
    }
}

static void hoa_optim_3d_dsp(t_hoa_optim_3d *x, t_object *dsp, short *count, double samplerate, long maxvectorsize, long flags)
{
    if(x->f_mode == hoa_sym_basic)
        object_method(dsp, gensym("dsp_add"), x, (method)hoa_optim_3d_perform_basic, 0, NULL);
    else if(x->f_mode == hoa_sym_maxRe)
        object_method(dsp, gensym("dsp_add"), x, (method)hoa_optim_3d_perform_maxRe, 0, NULL);
    else if(x->f_mode == hoa_sym_inPhase)
        object_method(dsp, gensym("dsp_add"), x, (method)hoa_optim_3d_perform_inPhase, 0, NULL);
}

static void hoa_optim_3d_symbol(t_hoa_optim_3d *x, t_symbol* s)
{
    if(s != x->f_mode && (s == hoa_sym_basic || s == hoa_sym_maxRe || s == hoa_sym_inPhase))
    {
        int state = canvas_suspend_dsp();
        ulong  order = x->f_optim->getDecompositionOrder();
        x->f_mode = s;
        delete x->f_optim;
        if(x->f_mode == hoa_sym_basic)
        {
            x->f_optim = new Optim<Hoa3d, t_sample>::Basic(order);
        }
        else if(x->f_mode == hoa_sym_maxRe)
        {
            x->f_optim = new Optim<Hoa3d, t_sample>::MaxRe(order);
        }
        else
        {
            x->f_optim  = new Optim<Hoa3d, t_sample>::InPhase(order);
        }
        canvas_resume_dsp(state);
    }
}

static void *hoa_optim_3d_new(t_symbol *s, int argc, t_atom *argv)
{
    ulong order           = 1;
    t_hoa_optim_3d *x   = (t_hoa_optim_3d *)eobj_new(hoa_optim_3d_class);
    t_binbuf *d         = binbuf_via_atoms(argc,argv);
    
    if(x && d)
    {
        x->f_mode   = hoa_sym_inPhase;
        if(atom_gettype(argv) == A_LONG)
        {
            order = ulong(pd_clip_minmax(atom_getlong(argv), 1, 10));
        }
        if(argc > 1 && atom_gettype(argv+1) == A_SYM && atom_getsymbol(argv+1) == hoa_sym_maxRe)
        {
            x->f_mode = hoa_sym_maxRe;
            x->f_optim = new Optim<Hoa3d, t_sample>::MaxRe(order);
        }
        else if(argc > 1 && atom_gettype(argv+1) == A_SYM && atom_getsymbol(argv+1) == hoa_sym_basic)
        {
            x->f_mode = hoa_sym_basic;
            x->f_optim = new Optim<Hoa3d, t_sample>::Basic(order);
        }
        else
        {
            x->f_optim  = new Optim<Hoa3d, t_sample>::InPhase(order);
        }
        
        eobj_dspsetup(x, long(x->f_optim->getNumberOfHarmonics()), long(x->f_optim->getNumberOfHarmonics()));
        x->f_ins = Signal<t_sample>::alloc(x->f_optim->getNumberOfHarmonics() * HOA_MAXBLKSIZE);
        x->f_outs = Signal<t_sample>::alloc(x->f_optim->getNumberOfHarmonics() * HOA_MAXBLKSIZE);
        
        return x;
    }
    
   	return NULL;
}

static void hoa_optim_3d_free(t_hoa_optim_3d *x)
{
    eobj_dspfree(x);
    delete x->f_optim;
    Signal<t_sample>::free(x->f_ins);
    Signal<t_sample>::free(x->f_outs);
}

extern "C" void setup_hoa0x2e3d0x2eoptim_tilde(void)
{
    t_eclass *c;
    c = eclass_new("hoa.3d.optim~",(method)hoa_optim_3d_new,(method)hoa_optim_3d_free,sizeof(t_hoa_optim_3d), 0L, A_GIMME, 0);
    
    eclass_dspinit(c);
    
    eclass_addmethod(c, (method)hoa_optim_3d_dsp,    "dsp",		A_CANT, 0);
    eclass_addmethod(c, (method)hoa_optim_3d_symbol, "symbol",  A_SYM,  0);
    
    eclass_register(CLASS_OBJ, c);
    hoa_optim_3d_class = c;
}

