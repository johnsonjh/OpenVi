#	$OpenBSD: merge.awk,v 1.3 2001/01/29 01:58:35 niklas Exp $

$1 == prev {
	printf ", %s", $2;
	next;
}
{
	if (NR != 1)
		printf "\n";
	printf "%s \t%s", $1, $2;
	prev = $1;
}
END {
	printf "\n"
}
