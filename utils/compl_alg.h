// Complexity checker API

// TODO: better to use static const int here!
#define				COMPL_MAX_METRIC		10
#define				COMPL_MAX_NAME			64

typedef struct {
	char			name[COMPL_MAX_NAME];
	union {
			int		value;
			double	dvalue;	// not user for now
	};
} Metric;

// TODO: make metrics as array[]
typedef struct {
	int			total;
	char	   *desc;
	FILE	   *out;
	FILE	   *err;
	// ------------------------------------------------------------------
	int			mcount;
	int			metrics[COMPL_MAX_METRIC];			// TODO: replace to heap memory
	char	   *metrics_desc[COMPL_MAX_METRIC];
} Compl;

void				compl_printall(Compl *st, const char *msg);
void				compl_free(Compl *st);
Compl				compl_newst(const char *desc, FILE *out, FILE *err);

Compl				compl_new(const char *desc){
	return compl_newst(desc, 0, 0);
}

static inline int	compl_pptotal(Compl *st){
	return ++st->total;
}

static inline int	compl_inctotal(Compl *st, int incval){
	return st->total += incval;
}

int					compl_setmetric(Compl *st, const char *mdesc, int initval);

static inline int	compl_ppmetric(Compl *st, int mid){
	return ++st->metrics[mid];
}

static inline int	compl_incmetric(Compl *st, int mid, int incval){
	return st->metrics[mid] += incval;
}

// ------------------------------------------------------------------------------------------------------
// End of Complexity checker

